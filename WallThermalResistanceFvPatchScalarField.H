/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    Foam::WallThermalResistanceFvPatchScalarField

Group
    grpInletBoundaryConditions grpOutletBoundaryConditions

Description
    This boundary condition ...


SourceFiles
WallThermalResistance
   WallThermalResistanceFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef WallThermalResistanceFvPatchScalarField_H
#define WallThermalResistanceFvPatchScalarField_H


#include "fixedValueFvPatchFields.H"
#include "WallThermalResistanceFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class WallThermalResistanceFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class WallThermalResistanceFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{

private:
	//Wall thermal resistance * area (units m^2*K/W)
	scalar R_wall;
	
	//Temperature at other side of wall (units K)
	scalar T_inf;
	

public:

    //- Runtime type information
    TypeName("WallThermalResistance");


    // Constructors

        //- Construct from patch and internal field
        WallThermalResistanceFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        WallThermalResistanceFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  WallThermalResistanceFvPatchScalarField
        //  onto a new patch
        WallThermalResistanceFvPatchScalarField
        (
            const WallThermalResistanceFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        WallThermalResistanceFvPatchScalarField
        (
            const WallThermalResistanceFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new WallThermalResistanceFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        WallThermalResistanceFvPatchScalarField
        (
            const WallThermalResistanceFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new WallThermalResistanceFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

		//- Copy parameters from another instance of this BC
		void copyParams( const WallThermalResistanceFvPatchScalarField& p );

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
