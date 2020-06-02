//Calculate thermal diffusivity field for faces:
const surfaceScalarField T_f = fvc::interpolate(T);
const scalarField& T_fp = T_f.primitiveField();
k_cpf.primitiveFieldRef() = k_T.interpolateValues( T_fp )/cp_T.interpolateValues( T_fp );


const surfaceScalarField::Boundary& T_fb = T_f.boundaryField();
surfaceScalarField::Boundary& k_cpfb = k_cpf.boundaryFieldRef();

forAll(k_cpfb, patchi)
{
	k_cpfb[patchi] == k_T.interpolateValues( T_fb[patchi] )/cp_T.interpolateValues( T_fb[patchi] );
}

//Weighting by alpha1 gradient and direction
const surfaceScalarField alpha1f = max(min(fvc::interpolate(alpha1),1.0),0.0);
const surfaceScalarField f_orient = mag( mixture.nHatf() ) / mesh.magSf();

const scalar gasEpsilon = 1E-4;


//k_cpf =   (1.0-f_orient)*( alpha1f*k_cpf + (1.0-alpha1f)*gasEpsilon*k_cpf )
//	     + f_orient/( alpha1f/k_cpf + (1.0-alpha1f)/(gasEpsilon*k_cpf) );
k_cpf =   alpha1f*k_cpf + (1.0-alpha1f)*gasEpsilon*k_cpf;


fvScalarMatrix EEqn
(
	fvm::ddt( max(min(alpha1,1.0),gasEpsilon)*rho1, h)
	+ fvm::div(rhoPhi1, h)
    ==
	fvm::laplacian(k_cpf, h)
);

EEqn.relax();

fvOptions.constrain(EEqn);

EEqn.solve();

//Bound solutions to be in the range of the property source data:
const scalar h_min =  T_h.first().first();
const scalar h_max =  T_h.last().first();

//Update temperature from new h
scalarField& hCells = h.primitiveFieldRef();
hCells = min( max( hCells, h_min*(1.0+SMALL) ), h_max*(1.0-SMALL) );

volScalarField::Boundary& hBf = h.boundaryFieldRef();
forAll(hBf, patchi)
{
	hBf[patchi] == min( max( hBf[patchi], h_min*(1.0+SMALL) ), h_max*(1.0-SMALL) );
}


//Update temperature from new h
scalarField& TCells = T.primitiveFieldRef();
TCells = T_h.interpolateValues( h.primitiveField() );
volScalarField::Boundary& TBf = T.boundaryFieldRef();
forAll(TBf, patchi)
{
	TBf[patchi] == T_h.interpolateValues( hBf[patchi] );
}
