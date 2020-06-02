/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2017 OpenCFD Ltd
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "PhaseChangeViscosity.H"
#include "addToRunTimeSelectionTable.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace viscosityModels
{
    defineTypeNameAndDebug(PhaseChangeViscosity, 0);

    addToRunTimeSelectionTable
    (
        viscosityModel,
        PhaseChangeViscosity,
        dictionary
    );
}
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //
/*
Foam::tmp<Foam::volScalarField>
Foam::viscosityModels::PhaseChangeViscosity::calcNu() const
{
    return ...;
}
*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::viscosityModels::PhaseChangeViscosity::PhaseChangeViscosity
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModel(name, viscosityProperties, U, phi),
	
	mu_T( fileName(viscosityProperties.lookup("muFileName")) ),
	rho_T( fileName(viscosityProperties.lookup("rhoFileName")) ),
	T_(U.mesh().lookupObject<volScalarField>("T")),
    nu_
    (
		IOobject
        (
            name,
            U.time().timeName(),
            U.mesh(),
			IOobject::NO_READ,
			IOobject::AUTO_WRITE
        ),
        U.mesh(),
        dimensionedScalar(dimLength*dimLength/dimTime, Zero)	
    )
{
	correct();
	
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::viscosityModels::PhaseChangeViscosity::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModel::read(viscosityProperties);
	
	//Note - not reinitializing lookup tables for mu and rho at this time (i.e., if the transportProperties dict gets modified at runtime and the read is called again)

    return true;
}

void Foam::viscosityModels::PhaseChangeViscosity::correct()
{
    scalarField& nuCells = nu_.primitiveFieldRef();
	nuCells =   mu_T.interpolateValues( T_.primitiveField() ) 
	          / rho_T.interpolateValues( T_.primitiveField() );
	
	
	volScalarField::Boundary& nuBf = nu_.boundaryFieldRef();
	const volScalarField::Boundary& TBf = T_.boundaryField();

	forAll(nuBf, patchi)
	{
		nuBf[patchi] ==   mu_T.interpolateValues( TBf[patchi] ) 
	                    / rho_T.interpolateValues( TBf[patchi] );
	}
}


// ************************************************************************* //
