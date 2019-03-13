{

THSSkeleton* sk=new THSSkeleton();
sk->SetHisto();
sk->SetWeights();

sk->CreateSelector ("histogrammer","/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output1/filepKp_all.root","HSParticles");
}
