double Wij(double d){
    double CE = 0.4, CI = 0.1,dE = 14.0,dI = 42.0;

    return CE*exp(-(pow(d, 2)/dE))- CI*exp(-(pow(d, 2)/dI));
}
