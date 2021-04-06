#include "vbstree.h"
#include "rooutil.h"
#include "cxxopts.h"

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    std::vector<float> st_bounds = {500., 1000., 1500., 2000.};
    std::vector<float> mjj_bounds = {500., 1000., 1500., 2000., 2500.};
    std::vector<float> detajj_bounds = {3, 3.5, 4, 4.5, 5, 5.5, 6};
    std::vector<float> ptbb_bounds = {0., 100., 150., 200., 250., 300., 350.};
    std::vector<float> leppt0_bounds = {0., 50., 75., 100., 125., 150., 175., 200.};

    TChain* tchain_sig = RooUtil::FileUtil::createTChain("variable", "vbshww_c2v_3.root");
    RooUtil::Looper<vbstree> looper_sig(tchain_sig, &vbs, -1);

    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>, float> wgts_sig;
    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>, float> cumul_wgts_sig;

    while (looper_sig.nextEvent())
    {
        // if (not (vbs.channel() == 0 or vbs.channel() == 1 or vbs.channel() == 2))
        //     continue;
        if (not (vbs.channel() == 3 or vbs.channel() == 4))
            continue;

        int ist = -1; 
        int imjj = -1; 
        int idetajj = -1; 
        int iptbb = -1; 
        int ileppt0 = -1; 
        for (unsigned int i = 0; i < st_bounds.size(); ++i) { float upb = i < st_bounds.size() - 1 ? st_bounds[i+1] : 99999999.; if (vbs.st() > st_bounds[i] and vbs.st() <= upb) { ist = i; break; } }
        for (unsigned int i = 0; i < mjj_bounds.size(); ++i) { float upb = i < mjj_bounds.size() - 1 ? mjj_bounds[i+1] : 99999999.; if (vbs.mjj() > mjj_bounds[i] and vbs.mjj() <= upb) { imjj = i; break; } }
        for (unsigned int i = 0; i < detajj_bounds.size(); ++i) { float upb = i < detajj_bounds.size() - 1 ? detajj_bounds[i+1] : 99999999.; if (vbs.detajj() > detajj_bounds[i] and vbs.detajj() <= upb) { idetajj = i; break; } }
        for (unsigned int i = 0; i < ptbb_bounds.size(); ++i) { float upb = i < ptbb_bounds.size() - 1 ? ptbb_bounds[i+1] : 99999999.; if (vbs.ptbb() > ptbb_bounds[i] and vbs.ptbb() <= upb) { iptbb = i; break; } }
        for (unsigned int i = 0; i < leppt0_bounds.size(); ++i) { float upb = i < leppt0_bounds.size() - 1 ? leppt0_bounds[i+1] : 99999999.; if (vbs.leppt0() > leppt0_bounds[i] and vbs.leppt0() <= upb) { ileppt0 = i; break; } }

        wgts_sig[std::make_tuple(ist, imjj, idetajj, iptbb, ileppt0)] += vbs.wgt();
    }

    for (unsigned int ist = 0; ist < st_bounds.size(); ++ist)
    {
        for (unsigned int imjj = 0; imjj < mjj_bounds.size(); ++imjj)
        {
            for (unsigned int idetajj = 0; idetajj < detajj_bounds.size(); ++idetajj)
            {
                for (unsigned int iptbb = 0; iptbb < ptbb_bounds.size(); ++iptbb)
                {
                    for (unsigned int ileppt0 = 0; ileppt0 < leppt0_bounds.size(); ++ileppt0)
                    {

                        float sum = 0;
                        for (unsigned int jst = ist; jst < st_bounds.size(); ++jst)
                        {
                            for (unsigned int jmjj = imjj; jmjj < mjj_bounds.size(); ++jmjj)
                            {
                                for (unsigned int jdetajj = idetajj; jdetajj < detajj_bounds.size(); ++jdetajj)
                                {
                                    for (unsigned int jptbb = iptbb; jptbb < ptbb_bounds.size(); ++jptbb)
                                    {
                                        for (unsigned int jleppt0 = ileppt0; jleppt0 < leppt0_bounds.size(); ++jleppt0)
                                        {
                                            sum += wgts_sig[std::make_tuple(jst, jmjj, jdetajj, jptbb, jleppt0)];
                                        }
                                    }
                                }
                            }
                        }

                        cumul_wgts_sig[std::make_tuple(ist, imjj, idetajj, iptbb, ileppt0)] = sum;

                    }
                }
            }
        }
    }

    TChain* tchain_bkg = RooUtil::FileUtil::createTChain("variable", "bkg.root");
    RooUtil::Looper<vbstree> looper_bkg(tchain_bkg, &vbs, -1);
    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>, float> wgts_bkg;
    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>, float> cumul_wgts_bkg;

    while (looper_bkg.nextEvent())
    {
        // if (not (vbs.channel() == 0 or vbs.channel() == 1 or vbs.channel() == 2))
        //     continue;
        if (not (vbs.channel() == 3 or vbs.channel() == 4))
            continue;

        int ist = -1; 
        int imjj = -1; 
        int idetajj = -1; 
        int iptbb = -1; 
        int ileppt0 = -1; 
        for (unsigned int i = 0; i < st_bounds.size(); ++i) { float upb = i < st_bounds.size() - 1 ? st_bounds[i+1] : 99999999.; if (vbs.st() > st_bounds[i] and vbs.st() <= upb) { ist = i; break; } }
        for (unsigned int i = 0; i < mjj_bounds.size(); ++i) { float upb = i < mjj_bounds.size() - 1 ? mjj_bounds[i+1] : 99999999.; if (vbs.mjj() > mjj_bounds[i] and vbs.mjj() <= upb) { imjj = i; break; } }
        for (unsigned int i = 0; i < detajj_bounds.size(); ++i) { float upb = i < detajj_bounds.size() - 1 ? detajj_bounds[i+1] : 99999999.; if (vbs.detajj() > detajj_bounds[i] and vbs.detajj() <= upb) { idetajj = i; break; } }
        for (unsigned int i = 0; i < ptbb_bounds.size(); ++i) { float upb = i < ptbb_bounds.size() - 1 ? ptbb_bounds[i+1] : 99999999.; if (vbs.ptbb() > ptbb_bounds[i] and vbs.ptbb() <= upb) { iptbb = i; break; } }
        for (unsigned int i = 0; i < leppt0_bounds.size(); ++i) { float upb = i < leppt0_bounds.size() - 1 ? leppt0_bounds[i+1] : 99999999.; if (vbs.leppt0() > leppt0_bounds[i] and vbs.leppt0() <= upb) { ileppt0 = i; break; } }

        wgts_bkg[std::make_tuple(ist, imjj, idetajj, iptbb, ileppt0)] += vbs.wgt();
    }

    for (unsigned int ist = 0; ist < st_bounds.size(); ++ist)
    {
        for (unsigned int imjj = 0; imjj < mjj_bounds.size(); ++imjj)
        {
            for (unsigned int idetajj = 0; idetajj < detajj_bounds.size(); ++idetajj)
            {
                for (unsigned int iptbb = 0; iptbb < ptbb_bounds.size(); ++iptbb)
                {
                    for (unsigned int ileppt0 = 0; ileppt0 < leppt0_bounds.size(); ++ileppt0)
                    {

                        float sum = 0;
                        for (unsigned int jst = ist; jst < st_bounds.size(); ++jst)
                        {
                            for (unsigned int jmjj = imjj; jmjj < mjj_bounds.size(); ++jmjj)
                            {
                                for (unsigned int jdetajj = idetajj; jdetajj < detajj_bounds.size(); ++jdetajj)
                                {
                                    for (unsigned int jptbb = iptbb; jptbb < ptbb_bounds.size(); ++jptbb)
                                    {
                                        for (unsigned int jleppt0 = ileppt0; jleppt0 < leppt0_bounds.size(); ++jleppt0)
                                        {
                                            sum += wgts_bkg[std::make_tuple(jst, jmjj, jdetajj, jptbb, jleppt0)];
                                        }
                                    }
                                }
                            }
                        }

                        cumul_wgts_bkg[std::make_tuple(ist, imjj, idetajj, iptbb, ileppt0)] = sum;

                    }
                }
            }
        }
    }

    for (const auto& [ k, v ] : cumul_wgts_sig)
    {
        int ist = std::get<0>(k);
        int imjj = std::get<1>(k);
        int idetajj = std::get<2>(k);
        int iptbb = std::get<3>(k);
        int ileppt0 = std::get<4>(k);
        float wgt_sig = v;
        float wgt_bkg = cumul_wgts_bkg[std::make_tuple(ist, imjj, idetajj, iptbb, ileppt0)];
        // std::cout <<  " ist: " << ist <<  " imjj: " << imjj <<  " idetajj: " << idetajj <<  " iptbb: " << iptbb <<  " ileppt0: " << ileppt0 <<  std::endl;
        float limit = RooUtil::StatUtil::cut_and_count_95percent_limit(wgt_sig, wgt_bkg, 0.3);
        std::cout <<  " st_bounds[ist]: " << st_bounds[ist] <<  " mjj_bounds[imjj]: " << mjj_bounds[imjj] <<  " detajj_bounds[idetajj]: " << detajj_bounds[idetajj] <<  " ptbb_bounds[iptbb]: " << ptbb_bounds[iptbb] <<  " leppt0_bounds[ileppt0]: " << leppt0_bounds[ileppt0] <<  " wgt_sig: " << wgt_sig <<  " wgt_bkg: " << wgt_bkg <<  " limit: " << limit <<  std::endl;
    }

}
