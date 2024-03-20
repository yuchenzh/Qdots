/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2019 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "Qdots.H"
#include "combustionModel.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(Qdots, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        Qdots,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::Qdots::Qdots
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    writeLocalObjects(obr_, false),
    phaseName_(word::null)
{
    read(dict);
    resetLocalObjectName(IOobject::groupName(type(), phaseName_));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::Qdots::~Qdots()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::Qdots::read
(
    const dictionary& dict
)
{
    fvMeshFunctionObject::read(dict);
    writeLocalObjects::read(dict);

    phaseName_ = dict.lookupOrDefault<word>("phase", word::null);

    return true;
}


bool Foam::functionObjects::Qdots::execute()
{
    word fieldName(IOobject::groupName(type(), phaseName_));

    const word modelName
    (
        IOobject::groupName
        (
            combustionModel::combustionPropertiesName,
            phaseName_
        )
    );
    IOobject QdotsFiles
    (
        fieldName,
        mesh_.time().timeName(),
        mesh_,
        IOobject::MUST_READ
    );
    if (QdotsFiles.headerOk())
    {
        Info<< "Reading Qdot field from file " << endl;
        tmp<volScalarField> tmpQdot
        (
            new volScalarField
            (
                IOobject
                (
                    fieldName,
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                mesh_
            )
        );
        return store(fieldName, tmpQdot);
    }

    return
        store
        (
            fieldName,
            mesh_.lookupObject<combustionModel>(modelName).Qdot()
        );
}


bool Foam::functionObjects::Qdots::write()
{
    return writeLocalObjects::write();
}


// ************************************************************************* //
