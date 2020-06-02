
Info<< "Generating h field from T\n" << endl;


//Find BC types for h field given those for T
const volScalarField::Boundary& tbf = T.boundaryField();

wordList hbbt(tbf.size(), word::null);

forAll(tbf, patchi)
{
/*
	if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
	{
		const fixedJumpFvPatchScalarField& pf =
			dynamic_cast<const fixedJumpFvPatchScalarField&>(tbf[patchi]);

		hbt[patchi] = pf.interfaceFieldType();
	}
	else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
	{
		const fixedJumpAMIFvPatchScalarField& pf =
			dynamic_cast<const fixedJumpAMIFvPatchScalarField&>
			(
				tbf[patchi]
			);

		hbbt[patchi] = pf.interfaceFieldType();
	}
*/
}


wordList hbt = tbf.types();

forAll(tbf, patchi)
{
	if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
	{
		hbt[patchi] = fixedValueFvPatchScalarField::typeName;
	}
/*
	else if
	(
		isA<zeroGradientFvPatchScalarField>(tbf[patchi])
	 || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
	)
	{
		hbt[patchi] = gradientEnergyFvPatchScalarField::typeName;
	}
	else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
	{
		hbt[patchi] = mixedEnergyFvPatchScalarField::typeName;
	}
	else if (isA<fixedJumpFvPatchScalarField>(tbf[patchi]))
	{
		hbt[patchi] = energyJumpFvPatchScalarField::typeName;
	}
	else if (isA<fixedJumpAMIFvPatchScalarField>(tbf[patchi]))
	{
		hbt[patchi] = energyJumpAMIFvPatchScalarField::typeName;
	}
*/
}


volScalarField h
(
	IOobject
	(
		"h",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
	),
	mesh,
	dimEnergy/dimMass,
	hbt,
	hbbt
);


//Set values in h:
scalarField& hCells = h.primitiveFieldRef();
hCells = h_T.interpolateValues( T.primitiveField() );

volScalarField::Boundary& hBf = h.boundaryFieldRef();

forAll(hBf, patchi)
{
	hBf[patchi] == h_T.interpolateValues( T.boundaryField()[patchi] );
}


//Boundary correction below
forAll(hBf, patchi)
{
//	Info<< hBf[patchi].type() << endl;

	if (isA<inletOutletFvPatchScalarField>(hBf[patchi]))
	{
		inletOutletFvPatchScalarField& hPatch = refCast<inletOutletFvPatchScalarField>(hBf[patchi]);
		const inletOutletFvPatchScalarField& TPatch = refCast<inletOutletFvPatchScalarField>( T.boundaryFieldRef()[patchi] );
		hPatch.refValue() = h_T.interpolateValues( TPatch.refValue() );
	}
	
	
/*
	if (isA<gradientEnergyFvPatchScalarField>(hBf[patchi]))
	{
		refCast<gradientEnergyFvPatchScalarField>(hBf[patchi]).gradient()
			= hBf[patchi].fvPatchField::snGrad();
	}
	else if (isA<mixedEnergyFvPatchScalarField>(hBf[patchi]))
	{
		refCast<mixedEnergyFvPatchScalarField>(hBf[patchi]).refGrad()
			= hBf[patchi].fvPatchField::snGrad();
	}
*/
}

