volScalarField dydt = fvc::ddt(y);
volScalarField tmp = Ksist*C2*dydt;

forAll(dcdt,i) {
    dcdt[i] = tmp[i];
}

// SuperS的得出方法需要参考模型
forAll(SuperS,i) {
    SuperS[i] = 0.01*dcdt[i];
}

Info<<"Solving pbe"<<endl;
populationBalance->solve();
