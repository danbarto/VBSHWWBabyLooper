import ast
import json
import pickle
import numpy as np

def bdt_to_json(fname, bst, features, labels=[]):
    """Return JSON of BDT model (Written by Nick Amin)

    Parameters
    ----------
    fname : str
        Desired name of output file
    bst : xgboost.core.Booster
        Trained BDT
    features : list[str]
        List of features used to train BDT
    labels : list[str], optional
        Class labels (default [])
    """
    if type(bst) == str:
        with open(bst, "r") as bst_pickle:
            bst = pickle.load(bst_pickle)
    buff = "[\n"
    trees = bst.get_dump()
    ntrees = len(trees)
    for itree, tree in enumerate(trees):
        prev_depth = 0
        depth = 0
        for line in tree.splitlines():
            depth = line.count("\t")
            nascending = prev_depth - depth
            (depth == prev_depth-1)
            # print ascending, depth, prev_depth
            prev_depth = depth
            parts = line.strip().split()
            padding = "    "*depth
            for iasc in range(nascending):
                buff += "{padding}]}},\n".format(padding="    "*(depth-iasc+1))
            if len(parts) == 1:  # leaf
                nodeid = int(parts[0].split(":")[0])
                leaf = float(parts[0].split("=")[-1])
                # print "leaf: ",depth,nodeid,val
                buff += """{padding}{{ "nodeid": {nodeid}, "leaf": {leaf} }},\n""".format(
                        padding=padding,
                        nodeid=nodeid,
                        leaf=leaf,
                        )
            else:
                nodeid = int(parts[0].split(":")[0])
                split, split_condition = parts[0].split(":")[1].replace("[","").replace("]","").split("<")
                split_condition = float(split_condition)
                yes, no, missing = map(lambda x:int(x.split("=")[-1]), parts[1].split(","))
                buff += """{padding}{{ "nodeid": {nodeid}, "depth": {depth}, "split": "{split}", "split_condition": {split_condition}, "yes": {yes}, "no": {no}, "missing": {missing}, "children": [\n""".format(
                        padding=padding,
                        nodeid=nodeid,
                        depth=depth,
                        split=split,
                        split_condition=split_condition,
                        yes=yes,
                        no=no,
                        missing=missing,
                        )
        for i in range(depth):
            padding = "    "*(max(depth-1,0))
            if i == 0:
                buff += "{padding}]}}".format(padding=padding)
            else:
                buff += "\n{padding}]}}".format(padding=padding)
            depth -= 1
        nlines = len(tree.splitlines())
        if (itree != len(trees)-1) and (nlines > 1):
            buff += ",\n"
    buff += "\n]"
    # print buff
    with open("DEBUG.txt", "w") as test_out:
        test_out.write(buff)
    to_dump = {
            "trees": list(ast.literal_eval(buff)),
            "features": features,
            "labels": map(int,np.array(labels).tolist()), # numpy array not json serializable
            }
    with open(fname, "w") as fhout:
        json.dump(to_dump,fhout,indent=2)

def bdt_json_to_cpp(fname_in, fname_out=None):
    """Return C function that mimics BDT output (Written by Nick Amin)

    Parameters
    ----------
    fname_in : str
        Name of input JSON file
    fname_out : str, optional
        Desired name of output C file
    """
    with open(fname_in, "r") as fhin:
        data = json.loads(fhin.read())
        trees = data["trees"]
        features = data["features"]
        labels = data["labels"]
    def get_leaf(entry, depth=0):
        if "leaf" in entry: 
            return entry["leaf"]
        splitvar = entry["split"]
        splitval = entry["split_condition"]
        yesnode = [c for c in entry["children"] if c["nodeid"] == entry["yes"]][0]
        nonode = [c for c in entry["children"] if c["nodeid"] == entry["no"]][0]
        return "({} < {} ? {} : {})".format(splitvar, splitval, get_leaf(yesnode, depth=depth+1), get_leaf(nonode, depth=depth+1))
    buff = ""
    multi = False
    if len(labels) > 0:
        multi = True
        ntrees_per_class = len(trees) // len(labels)
        nclasses = len(labels)
    if multi:
        buff += "std::vector<float> get_prediction({}) {{\n".format(",".join(map(lambda x: "float {}".format(x), features)))
        for ic in labels:
            buff += "  float w_{} = 0.;\n".format(ic)
        for itree,j in enumerate(trees):
            iclass = int(labels[itree % nclasses])
            buff += "  w_{} += {};\n".format(iclass,get_leaf(j))
        buff += "  float w_sum = {};\n".format("+".join("exp(w_{})".format(ic) for ic in labels))
        for ic in labels:
            buff += "  w_{0} = exp(w_{0})/w_sum;\n".format(ic)
        buff += "  return {{ {} }};\n".format(",".join("w_{}".format(ic) for ic in labels))
    else:
        buff += "float get_prediction({}) {{\n".format(",".join(map(lambda x: "float {}".format(x), features)))
        buff += "  float w = 0.;\n"
        for itree,j in enumerate(trees):
            buff += "  w += {};\n".format(get_leaf(j))
        buff += "  return 1.0/(1.0+exp(-1.0*w));\n"
    buff += "}"
    if fname_out:
        with open(fname_out, "w") as fhout:
            fhout.write(buff)
    else:
        return buff

if __name__ == "__main__":
    # with open("features.json", "r") as features_json:
    #     features = json.load(features_json)
    # with open("bdt.pkl", "r") as bdt_pickle:
    #     bst = pickle.load(bdt_pickle)
    # bdt_to_json("bdt.json", bst, features)
    # json_to_C("bdt.json", fname_out="bdt.cc")

    with open("kitchen_sink_features.json", "r") as features_json:
        features = json.load(features_json)
    with open("kitchen_sink_bdt.pkl", "r") as bdt_pickle:
        bdt = pickle.load(bdt_pickle)
    bdt_to_json("kitchen_sink_bdt.json", bdt, features)
    bdt_json_to_cpp("kitchen_sink_bdt.json", fname_out="kitchen_sink_bdt.cc")
