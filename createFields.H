Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

IOdictionary ReactionProperties
(
    IOobject
    (
        "ReactionProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

dimensionedScalar C1
(
    "C1",
    dimMoles*inv(dimVolume),
    ReactionProperties
);

dimensionedScalar C2
(
    "C2",
    dimMoles*inv(dimVolume),
    ReactionProperties
);

dimensionedScalar k
(
    "k",
    dimVolume*inv(dimMoles*dimTime),
    ReactionProperties
);

Info<< "Reading field Mixture fraction Ksi\n" << endl;
volScalarField Ksi
(
    IOobject
    (
        "Ksi",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
    IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field for Mixture fraction variance" << endl;
volScalarField KsiVar
(
    IOobject
    (
    "KsiVar",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
    IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field for reaction progress variable" << endl;
volScalarField y
(
    IOobject
    (
    "y",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
    IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField dydt(y);
volScalarField yOld(y);

#include "createPhi.H"


label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, piso.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());


singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);


scalar mfSct = 0.7;
scalar mfCmu = 0.09;
scalar mfCphi = 2;


#include "createMRF.H"
#include "createFvOptions.H"

//以下为pbe所需, 通过mesh.lookup传递给pbe

volScalarField dcdt
(
    IOobject
    (
    "dcdt",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
    IOobject::AUTO_WRITE
    ),
    mesh,
    dimMoles/dimTime
);

volScalarField SuperS
(
    IOobject
    (
    "SuperS",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
    IOobject::AUTO_WRITE
    ),
    mesh,
    dimless
);

SuperS = 0.0;

volScalarField SuperSTmp(SuperS);

volScalarField averageSize
(
    IOobject
    (
    "AveragedSize",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
    IOobject::AUTO_WRITE
    ),
    mesh,
    dimLength
);

forAll(averageSize, i) {
    averageSize[i] = 0.0;
}

IOdictionary populationBalanceProperties
(
    IOobject
    (
        "populationBalanceProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

autoPtr<populationBalanceModel> populationBalance
(
    populationBalanceModel::New
    (
        "populationBalance", populationBalanceProperties, phi
    )
);
