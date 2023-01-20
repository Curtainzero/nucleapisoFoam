dimensionedScalar Ksist = C1/(C1+C2);

// Info << "Ksist:  " << Ksist.value() << endl;

// Info<<"Reaction progress equation establishing"<<endl;

scalar dt = runTime.deltaTValue();
//dimensionedScalar dt("time",dimensionSet( 0, 0, 1, 0, 0, 0, 0),runTime.deltaTValue());
// Info<<"Calculating coef"<<endl;
volScalarField temp = (Ksi*(1-Ksi))+SMALL;
volScalarField coef1 = Ksist*k*C2*(1-KsiVar/temp);
volScalarField coef2 = (1-Ksi)/(1-Ksist);
volScalarField coef3 = Ksi/Ksist;

// To do：以下ode部分可优化，能算就行先不动

volScalarField::Internal yin = y.internalField();
// volScalarField::Internal c1in = coef1.internalField();
// volScalarField::Internal c2in = coef2.internalField();
// volScalarField::Internal c3in = coef3.internalField();


// Info<<"Calculating derivative"<<endl;
double yp;
double ynext;
double c1;
double c2;
double c3;
forAll(yin, i) {
    yp = y[i];
    c1 = coef1[i];
    c2 = coef2[i];
    c3 = coef3[i];
    ynext = yp + dt*c1*(c2-yp)*(c3-yp);
    if (ynext > 1) {
        ynext = 1.0;
    } else if (ynext < 0) {
        ynext = 0.0;
    }
    y[i] = ynext;
    dydt[i] = c1*(c2-yp)*(c3-yp);
}

volScalarField tmp = Ksist*C2*dydt;

forAll(dcdt,i) {
    dcdt[i] = posPart(tmp[i]);
}



// SuperS的得出方法需要参考模型，先这样假设
forAll(SuperS,i) {
    SuperS[i] = 10000*dcdt[i];
}






