import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pickle
import json
from tqdm import tqdm
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_curve
import xgboost as xgb #BDT

class XGBoostParamScan:
    def __init__(self, x_train, x_test, y_train, y_test, features, weights_train=[], weights_test=[]):
        # Parameter configurations to scan over
        self.param_configs = [{}]
        self.summary = {}
        # Results to parse later
        self.results = []
        # BDT Input
        if len(weights_train) == 0:
            weights_train = np.ones(x_train.shape)
        if len(weights_test) == 0:
            weights_test = np.ones(x_train.shape)
        self.x_train = x_train
        self.y_train = y_train
        self.weights_train = weights_train
        self.x_test = x_test
        self.y_test = y_test
        self.weights_test = weights_test
        self.dtrain = xgb.DMatrix(x_train[features], label=y_train, weight=np.abs(weights_train))
        self.dtest = xgb.DMatrix(x_test[features], label=y_test, weight=np.abs(weights_test))
        self.evallist = [(self.dtrain,"train"), (self.dtest,"test")]
        
    def addParam(self, name, values):
        self.summary[name] = len(values)
        # Generate new parameter configurations
        new_param_configs = []
        for param_config in self.param_configs:
            for value in values:
                new_param_config = param_config.copy()
                new_param_config[name] = value
                new_param_configs.append(new_param_config)
        self.param_configs = new_param_configs
        return

    def print_summary(self):
        printed_lines = []
        for name, n_values in self.summary.iteritems():
            line_to_print = "  + {0} ({1} values)".format(name, n_values)
            print(line_to_print)
            printed_lines.append(line_to_print)
        print("  __"+"".join(["_" for _ in range(max([len(l) for l in printed_lines]))]))
        print("  = {} total configurations".format(len(self.param_configs)))
        return
        
    def run(self, n_rounds=200, early_stopping_rounds=None, n_threads=1, eval_metric="auc"):
        for param_config in tqdm(self.param_configs):
            param_config["verbosity"] = 0
            param_config["silent"] = 1
            param_config["nthread"] = n_threads
            param_config["eval_metric"] = eval_metric
            param_config["objective"] = "binary:logistic"
            these_results = {}
            bdt = xgb.train(
                param_config.items(), 
                self.dtrain, 
                n_rounds, 
                self.evallist,
                evals_result=these_results,
                early_stopping_rounds=early_stopping_rounds, 
                verbose_eval=False
            )
            # Get predictions
            preds_train = bdt.predict(xgb.DMatrix(self.x_train))
            preds_test = bdt.predict(xgb.DMatrix(self.x_test))
            # Get test/train AUC
            test_fpr, test_tpr, test_thresh = roc_curve(y_test, preds_test)
            test_auc = np.trapz(test_tpr, test_fpr)
            train_fpr, train_tpr, train_thresh = roc_curve(y_train, preds_train)
            train_auc = np.trapz(train_tpr, train_fpr)
            # Store additional metrics
            these_results["test"]["avg_"+eval_metric] = np.mean(these_results["test"][eval_metric])
            these_results["test"]["auc"] = test_auc
            these_results["train"]["avg_"+eval_metric] = np.mean(these_results["train"][eval_metric])
            these_results["train"]["auc"] = train_auc
            these_results["delta_auc"] = abs(test_auc - train_auc)
            these_results["param_config"] = param_config
            # Save results
            self.results.append(these_results)
        # Write results to pickle file
        with open("results.pkl", "w") as f_out:
            pickle.dump(self.results, f_out)
        return

if __name__ == "__main__":
    # Get data
    ntuple_home = "../../hadds/extraJetsPreprocBDT_Run2"
    samples = ["ttw", "ttz"]
    loaded_ntuples = []
    df = []
    for sample in samples:
        # Retrieve TTree
        ntuple_path = ntuple_home+"/{}.root".format(sample)
        f = uproot.open(ntuple_path)
        tree = f.get("variable")
        temp_df = tree.pandas.df(branches=["MVA_*"], flatten=True)
        df.append(temp_df)
        loaded_ntuples.append(ntuple_path)
    print(" -- {} ntuples loaded -- \n".format(len(df)))
    df = pd.concat(df)
    # Remove "MVA" prefix from column names
    df.columns = df.columns.str.split("MVA_").str.get(1)
    # Make a preliminary cut on Mjj
    df = df[np.abs(df.extra_jet_pair_M - 80.379) < 30]
    print("    "+"\n  + ".join(loaded_ntuples))
    print("  __"+"".join("_" for _ in range(max([len(s) for s in loaded_ntuples]))))
    print("  = {} total events (after preselection)".format(df.shape[0]))
    # Get bookkeeping columns to exclude in training
    excludes = [
        "run", 
        "event", 
        "lumi", 
        "extra_jet_pair_is_match", 
        "bjet_pair_ld_deepjet",
        "bjet_pair_tr_deepjet",
        "bjet_pair_ld_eta",
        "bjet_pair_tr_eta",
        "lep_pair_ld_id",
        "lep_pair_tr_id",
        "lep_pair_ld_eta",
        "lep_pair_tr_eta",
        "vbs_pair_ld_eta",
        "vbs_pair_tr_eta",
        "extra_jet_pair_ld_deepjet",
        "extra_jet_pair_tr_deepjet",
    ]
    # Collect selected features
    features = sorted(list(set(df.columns.values)-set(excludes)))
    print("\n -- Features selected -- \n")
    print("    "+"\n  + ".join(features))
    print("  __"+"".join("_" for _ in range(max([len(s) for s in features]))))
    print("  = {} total features".format(len(features)))
    # Split dataframe for training and testing
    dummy_weights = np.ones(df.extra_jet_pair_is_match.size) # in place of dfs["weights"]
    x_train, x_test, y_train, y_test, weights_train, weights_test = train_test_split(
        df,
        df.extra_jet_pair_is_match,
        dummy_weights,
        test_size=0.25, 
        random_state=42,
    )
    events_train, events_test = train_test_split(df.event.unique(), test_size=0.25, random_state=42)
    x_train = df[np.isin(df.event, events_train)]
    y_train = x_train.extra_jet_pair_is_match
    weights_train = np.ones(y_train.size)
    x_test = df[np.isin(df.event, events_test)]
    y_test = x_test.extra_jet_pair_is_match
    weights_test = np.ones(y_test.size)
    print("\n -- Data prepared -- \n")
    print("    {} training events".format(x_train.shape[0]))
    print("    {} testing events".format(x_test.shape[0]))

    # Initialize parameter scan object
    param_scan = XGBoostParamScan(x_train[features], x_test[features], y_train, y_test, features)
    print("\n -- Scan initialized -- \n")
    # Add variable parameters
    param_scan.addParam("max_depth", [3])
    param_scan.addParam("alpha", np.linspace(0.0, 2.5, 11))
    param_scan.addParam("lambda", np.linspace(1.0, 2.0, 11))
    param_scan.addParam("eta", np.linspace(0.2, 0.5, 5))
    param_scan.addParam("gamma", np.linspace(0.0, 2.0, 11))
    param_scan.addParam("subsample", np.linspace(0.3, 0.6, 9))
    # Add constant parameters
    sumw_pos = np.abs(param_scan.dtrain.get_weight()[param_scan.dtrain.get_label()==1]).sum()
    sumw_neg = np.abs(param_scan.dtrain.get_weight()[param_scan.dtrain.get_label()==1]).sum()
    param_scan.addParam("scale_pos_weight", [sumw_neg/sumw_pos])
    param_scan.addParam("min_child_weight", [200.0])

    n_configs = len(param_scan.param_configs)
    param_scan.print_summary()
    print("    At 10 it/sec, this will take {} minutes.".format(n_configs/10./60.))

    # param_scan.run(n_rounds=500, early_stopping_rounds=30, n_threads=12, eval_metric="error")
