double Wij2(double d){
    double CE = 0.4, CI = 0.1, dE = 14.0, dI = 42.0;

    double wynik = CE*exp(-(pow(d, 2)/dE))- CI*exp(-(pow(d, 2)/dI));

    double d0 = 5.4;
    double dm = 15;
    if (d <= d0){
        if (wynik < 0){
            return 0;
        }
    }
    if((dm > d)&&  (d > d0)){
        //wynik = 0; //zerujê w d > 5.4
    }
    return wynik;
}
