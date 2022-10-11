volScalarField dydt = (y - yOld)/dt;
volScalarField tmp = Ksist*C2*dydt;

forAll(dcdt,i) {
    dcdt[i] = tmp[i];
}

// SuperS的得出方法需要参考模型，先这样假设
forAll(SuperS,i) {
    SuperS[i] = 100*dcdt[i];
}


// Info<<"Solving pbe"<<endl;
populationBalance->solve();
