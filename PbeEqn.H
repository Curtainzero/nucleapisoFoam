

// Info<<"Solving pbe"<<endl;
populationBalance->solve();

const volScalarField& totalN = mesh.lookupObject<volScalarField>("moment.0.populationBalance");
const volScalarField& totalL = mesh.lookupObject<volScalarField>("moment.1.populationBalance");
Info << "then" << endl;

forAll(averageSize,i){
    if (totalN[i]<1.0) {
        averageSize[i] = 0.0;
    } else {
        averageSize[i] = totalL[i]*1e9/totalN[i];
    }
    
}

//averageSize = totalL/totalN;

