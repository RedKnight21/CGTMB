#include "baseVtkWindow.h"

// ----------------------------------------------------------------------------
// Constructor
BaseVtkWindow::BaseVtkWindow(MasterData *mdIn)
{
    md = mdIn;
    textProperty1->SetColor(0.01, 0.01, 0.01);
    //textProperty1->SetFontFamily(1);

//    cout << "Font family: " << textProperty1->GetFontFamily() << endl;
//    cout << "Font family: " << textProperty1->GetFontFamilyAsString(0) << endl;
//    cout << "Font family: " << textProperty1->GetFontFamilyAsString(1) << endl;
//    cout << "Font family: " << textProperty1->GetFontFamilyAsString(2) << endl;

    textProperty1->BoldOn();
    textProperty1->SetFontSize(12);

    axes->GetXAxisCaptionActor2D()->SetCaptionTextProperty(textProperty1);
    axes->GetYAxisCaptionActor2D()->SetCaptionTextProperty(textProperty1);
    axes->GetZAxisCaptionActor2D()->SetCaptionTextProperty(textProperty1);

    axes->SetShaftTypeToCylinder();
    noBooleanOps = false;

//    vtkProperty *vp = contourActor->GetProperty();
//    vp->Print(cout);

    zoneProperty1->SetAmbient(0.1);
    zoneProperty1->SetDiffuse(0.8);
    backFaceStyle = 0;

    whiteWashFactor = 0.3;
    transparancy    = 1.0;
    viewMode        = 0;

    goOrientationMarkerWidget = true;
    goCamOrientationWidget = true;
}
// ----------------------------------------------------------------------------
BaseVtkWindow::~BaseVtkWindow()
{
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::resetForProject()
{
    xpt[0] = -100000.0;
    xpt[1] =       0.0;
    xpt[2] =       0.0;

    ypt[1] = -100000.0;
    ypt[0] =       0.0;
    ypt[2] =       0.0;

    zpt[2] = -100000.0;
    zpt[1] =       0.0;
    zpt[0] =       0.0;

    viewMode     = 0;
    noBooleanOps = false;
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::toggleOrientationWidget()
{
    if (goOrientationMarkerWidget)
    {
        goOrientationMarkerWidget = false;
        orientationWidget->Off();
    }
    else
    {
        goOrientationMarkerWidget = true;
        orientationWidget->On();
    }
    ren->Clear();
    renderWindow->Render();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::toggleCamOrientationWidget()
{
    if (goCamOrientationWidget)
    {
        goCamOrientationWidget = false;
        camOrientManipulator->Off();
    }
    else
    {
        goCamOrientationWidget = true;
        camOrientManipulator->On();
    }
    ren->Clear();
    renderWindow->Render();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::makeVtkWidget(QSplitter *splitter)
{
    // Geometry
    vtkNew<vtkVectorText> text;
    text->SetText("CGTMB");
    vtkNew<vtkElevationFilter> elevation;
    elevation->SetInputConnection(text->GetOutputPort());
    elevation->SetLowPoint(0, 0, 0);
    elevation->SetHighPoint(10, 0, 0);

    // Mapper
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(elevation->GetOutputPort());

    // Actor in scene
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Add Actor to renderer
    ren->AddActor(actor);
    ren->SetBackground(0.9, 0.9, 0.9);

    qvtkWidget = new QVTKRenderWidget(splitter);
    qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);

    sizePolicy.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
    qvtkWidget->setSizePolicy(sizePolicy);
//    qvtkWidget->setMinimumSize(QSize(700, 700));
    qvtkWidget->setMaximumSize(QSize(2000, 2000));

    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(ren);

    renderWindow->Render();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeDefaultView()
{
    clearAllActors();
    resetAllVectors();
    camOrientManipulator->SetEnabled(0);
    orientationWidget->SetEnabled(0);

    // Geometry
    vtkNew<vtkVectorText> text;
    text->SetText("CGTMB");
    vtkNew<vtkElevationFilter> elevation;
    elevation->SetInputConnection(text->GetOutputPort());
    elevation->SetLowPoint(0, 0, 0);
    elevation->SetHighPoint(10, 0, 0);

    // Mapper
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(elevation->GetOutputPort());

    // Actor in scene
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Add Actor to renderer
    ren->AddActor(actor);
    ren->SetBackground(0.98, 0.98, 0.98);

    vtkNew<vtkCamera> camera;
    camera->Azimuth(0);
    camera->Elevation(0);
    ren->SetActiveCamera( camera );
    ren->ResetCamera();
    iRen = renderWindow->GetInteractor();

    renderWindow->Render();
    iRen->Initialize();
    renderWindow->Render();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::clearAllActors()
{
    // Retreive all of the actors in the vktRenderer and
    // remove all of them.
    vtkActorCollection *ac = ren->GetActors();
    vtkActor *actor;
    for(ac->InitTraversal(); (actor = ac->GetNextItem()) != NULL;)
    {
        ren->RemoveActor(actor);
    }

    ren->Clear();
    renderWindow->Render();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::resetAllVectors()
{
    checkBools.clear();
    srcs.clear();
    bools.clear();
    zoneMappers.clear();
    zoneDisplay.clear();
    ugridDisplay.clear();
    ugridMappers.clear();
    ugrids.clear();

    ugridColorR.clear();
    ugridColorG.clear();
    ugridColorB.clear();

    colorR.clear();
    colorG.clear();
    colorB.clear();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::updateLight(double *values, int style)
{
    zoneProperty1->SetAmbient(values[0]);
    zoneProperty1->SetDiffuse(values[1]);

    backFaceStyle   = style;
    whiteWashFactor = values[2];
    transparancy    = values[3];
    double color[3];

    // Retreive all of the actors in the vktRenderer and
    // update their property.
    vtkActorCollection *ac = ren->GetActors();
    vtkActor *actor;
    for(ac->InitTraversal(); (actor = ac->GetNextItem()) != NULL;)
    {
        actor->GetProperty()->SetAmbient(values[0]);
        actor->GetProperty()->SetDiffuse(values[1]);
        actor->GetProperty()->GetColor(color);
        actor->GetProperty()->SetOpacity(transparancy);

        if (backFaceStyle > 0)
        {
            color[0] = (1.0 - whiteWashFactor) * color[0] + whiteWashFactor;
            color[1] = (1.0 - whiteWashFactor) * color[1] + whiteWashFactor;
            color[2] = (1.0 - whiteWashFactor) * color[2] + whiteWashFactor;
        }
        actor->GetBackfaceProperty()->SetColor(color);
        actor->GetBackfaceProperty()->SetAmbient(values[0]);
        actor->GetBackfaceProperty()->SetDiffuse(values[1]);

//        vtkProperty *vp = actor->GetProperty();
//        vp->Print(cout);

    }
    ren->Clear();
    renderWindow->Render();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeAxisTriad()
{
    iRen = renderWindow->GetInteractor();

    double rgba[4]{0.0, 0.0, 0.0, 0.0};
    colors->GetColor("Carrot", rgba);
    orientationWidget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    orientationWidget->SetOrientationMarker(axes);
    orientationWidget->SetViewport(0.0, 0.0, 0.2, 0.2);

    orientationWidget->SetInteractor(iRen);
    orientationWidget->SetEnabled(1);
    orientationWidget->InteractiveOn();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeCameraOrientationWidget()
{
    camOrientManipulator->SetParentRenderer(ren);
    // Enable the widget.
    camOrientManipulator->On();
}
// ----------------------------------------------------------------------------

void BaseVtkWindow::updateEntireModel(Bounds *inBounds, QVector<ReadFileAbaqus> *abaqus)
{
    mBounds = inBounds;
    mBounds->setStartingLimits();

    QVector<CGBodyBasics> vtkBodyBasics;
    md->makeBodyBasics(&vtkBodyBasics);

    QVector<CellData> vtkCellList;
    md->makeCellList(&vtkCellList);

    clearAllActors();
    resetAllVectors();

    makeAxisTriad();
    makeCameraOrientationWidget();

    zoneMappers.clear();
    zoneDisplay.clear();
    codeZoneIn.clear();
    codeZoneOr.clear();

    // Make all of the bodies only once.
    for(int i=0; i<vtkBodyBasics.size(); ++i)
    {
        makeBody( (vtkBodyBasics)[i] );
    }

    numCodeZones.clear();
    zoneSpecs.clear();
    colorR.clear();
    colorG.clear();
    colorB.clear();

    for(int cell=0; cell<vtkCellList.size(); ++cell)
    {
        checkCellBooleanOperations(&vtkCellList[cell].zoneOps);
        makeZoneMappersBoolean(&vtkCellList[cell].zoneOps, vtkCellList[cell].color,
                               cell, vtkCellList[cell].display);
    }

    if (abaqus->size() > 0)
    {
        updateAllUGrids(abaqus);
    }

    viewMode = 0;
    makeZoneActors2();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::setupClipPlanes()
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto normals = vtkSmartPointer<vtkDoubleArray>::New();
    vtkNew<vtkPoints> clipPts;
    normals->SetNumberOfComponents(3);
    normals->InsertNextTuple(xnorm);
    normals->InsertNextTuple(ynorm);
    normals->InsertNextTuple(znorm);

    clipPts->InsertNextPoint(xpt);
    clipPts->InsertNextPoint(ypt);
    clipPts->InsertNextPoint(zpt);

    vtkNew<vtkPlanes> clipPlanes;
    clipPlanes->SetNormals(normals);
    clipPlanes->SetPoints(clipPts);
}


// ----------------------------------------------------------------------------
void BaseVtkWindow::updateAllUGrids(QVector<ReadFileAbaqus> *abaqus)
{
    ugridColorR.clear();
    ugridColorG.clear();
    ugridColorB.clear();
    ugrids.clear();
    ugridDisplay.clear();
    ugridMappers.clear();

    for(auto abq=0; abq < abaqus->size(); ++abq)
    {
        md->getInstanceDataColors(&ugridColorR, &ugridColorG, &ugridColorB);

        makeUMeshModel( &(*abaqus)[abq] );
    }

    makeUMeshMappers();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::setupClipPlanes(vtkSmartPointer<vtkPlanes> clipPlanes)
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto normals = vtkSmartPointer<vtkDoubleArray>::New();
    vtkNew<vtkPoints> clipPts;
    normals->SetNumberOfComponents(3);
    normals->InsertNextTuple(xnorm);
    normals->InsertNextTuple(ynorm);
    normals->InsertNextTuple(znorm);

    clipPts->InsertNextPoint(xpt);
    clipPts->InsertNextPoint(ypt);
    clipPts->InsertNextPoint(zpt);

    clipPlanes->SetNormals(normals);
    clipPlanes->SetPoints(clipPts);
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::checkCellBooleanOperations(QVector<CGZoneOps>  *cgZoneOpsLocal)
{
    int i1;
    int k1;
    int action;

    std::vector<int> or_info;
    std::vector<int> in_info;


    i1 = 0;
    for (int i=0; i<cgZoneOpsLocal->size(); ++i)
    {
        or_info.push_back(i1);
        in_info.push_back(i1);
    }

    // Ensure that 1st / leading operation is an AND and not an OR.
    if ((*cgZoneOpsLocal)[0].boolOp == 1)
        (*cgZoneOpsLocal)[0].boolOp = 0;

    // Find the number of bodies in each code zone.
    int nCodeZones = 0;
    for (int i=0; i<cgZoneOpsLocal->size(); ++i)
    {
        if ( (*cgZoneOpsLocal)[i].boolOp == 1)
        {
            ++nCodeZones;
            ++or_info[nCodeZones];
        }
        else
            ++or_info[nCodeZones];

        // Find the number inside.
        if ((*cgZoneOpsLocal)[i].senseOp < 0)
            ++in_info[nCodeZones];
    }
    ++nCodeZones;

    numCodeZones.push_back(nCodeZones);
    codeZoneIn.push_back(in_info);
    codeZoneOr.push_back(or_info);

    QVector<CGZoneOps> workingOps;

    std::vector<int> check;

    int start = 0;
    for (int i=0; i<nCodeZones; ++i)
    {
        workingOps.clear();
        // Grab the data for the code zone.
        for(int k=start; k < (or_info[i] + start); ++k)
        {
            workingOps.push_back( (*cgZoneOpsLocal)[k] );
        }
        // Sort in order by body number with its sense.
        std::sort(workingOps.begin(), workingOps.end());

        check.clear();

        // Only need to check boolean operations if there is more than 1 body.
        if (or_info[i] > 1)
        {
            int n = or_info[i];

            // Initialize 1-D interaction matrix.
            for(int k=0; k < (n*n) ; ++k) check.push_back(-1);
            for(int k=0; k < n; ++k)
            {
                i1 = k*n + k;
                check[i1] = 0;
            }

            int bnk;    // body number k
            int bnm;    // body number m

            for (int k=0; k<workingOps.size(); ++k)
            {
                bnk = workingOps[k].bodyNum - 1;

                for (int m=k+1; m<workingOps.size(); ++m)
                {
                    bnm = workingOps[m].bodyNum - 1;

                    vtkSmartPointer<sssBooleanOperationCheck> boolFilter =
                        vtkSmartPointer<sssBooleanOperationCheck>::New();

                    if ((workingOps[k].senseOp + workingOps[m].senseOp) == 0)
                    {
                        if (workingOps[k].senseOp == -1)
                        {
                            i1 = 0;
                            k1 = 1;
                        }
                        else
                        {
                            i1 = 1;
                            k1 = 0;
                        }
                        action = sssBooleanOperationCheck::VTK_DIFFERENCE;
                    }
                    else if ((workingOps[k].senseOp + workingOps[m].senseOp) == 2)
                    {
                        i1 = 0;
                        k1 = 1;
                        action = sssBooleanOperationCheck::VTK_UNION;
                    }
                    else
                    {
                        i1 = 0;
                        k1 = 1;
                        action = sssBooleanOperationCheck::VTK_INTERSECTION;
                    }

                    boolFilter->SetOperation( action );
                    boolFilter->SetInputConnection( i1, (srcs)[bnk]->GetOutputPort() );
                    boolFilter->SetInputConnection( k1, (srcs)[bnm]->GetOutputPort() );

                    boolFilter->GlobalWarningDisplayOff();
                    bool b1 = false;
                    b1 = boolFilter->GetExecutive()->Update();
                    if (b1)
                    {
                        i1 = k * n + k;
                        ++check[i1];
                        i1 += (m - k);

                        check[i1] = action;
                        i1 = m * n + m;

                        ++check[i1];
                        i1 = m * n + k;

                        check[i1] = action;
                    }
                }               // -- end m-loop
            }                   // -- end k-loop

            // Some input checks for sense.
            for(int k=0; k<n; ++k)
            {
                i1 = k*n + k;
                if ((check[i1] == 0) && (or_info[i] > 1))
                {
                    if (workingOps[k].senseOp == -1)
                    {
                        if (in_info[i] > 1)
                        {
                            userWarning(0, workingOps[k].bodyNum);
                        }
                    }
                    else if (workingOps[k].senseOp == 1)
                    {
                        if (in_info[i] == 0)
                        {
                            userWarning(1, workingOps[k].bodyNum);
                        }
                    }
                }               // -- end if check[i1] == 0
            }                   // -- end k-llop
        }                       // -- or_info > 1
        else
        {
            check.push_back(0);
        }

        // Save interaction matrix here.
        zoneSpecs.push_back(check);

        // Increment for next code zone.
        start += or_info[i];
    }                           // -- end i-loop over code zones
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneMappersBoolean(QVector<CGZoneOps>  *cgZoneOpsLocal, QColor &color,
                                           const int cell, const bool display)
{
    std::vector < vtkSmartPointer < vtkPolyData >> maps;
    QVector<CGZoneOps> workingOps;

    double b[6];

    int bnk;    // body number k
    int bnm;    // body number m
    int start = 0;
    for (int i=0; i<numCodeZones[cell]; ++i)
    {
        vtkNew<vtkPolyDataNormals> normals1;
        vtkNew<vtkOpenGLPolyDataMapper> mapper;

        workingOps.clear();
        // Grab the data for the code zone.
        for(int k=start; k < (codeZoneOr[cell][i] + start); ++k)
        {
            workingOps.push_back( (*cgZoneOpsLocal)[k] );
        }
        // Sort in order by body number with its sense.
        std::sort(workingOps.begin(), workingOps.end());

        int nBodies = codeZoneOr[cell][i];

        maps.clear();
        if (nBodies == 1)
        {
            bnk = workingOps[0].bodyNum - 1;
            normals1->SetInputConnection((srcs)[bnk]->GetOutputPort( ));
        }
        else
        {
            int march = -1;
            bools.clear();
            for (int k=0; k<nBodies; ++k)
            {
                bnk = workingOps[k].bodyNum - 1;

                int i1;
                i1 = nBodies *  k + k;
                if (zoneSpecs[i][i1] == 0)
                {
                    // Process body that has no interaction with the rest.
                    vtkNew<vtkPolyData> map;
                    map->DeepCopy((srcs)[k]->GetOutput( ));
                    map->ComputeCellsBounds();
                    map->ComputeBounds();
                    map->GetBounds(b);
                    maps.push_back(map);
                    updateBounds(b);
                    continue;
                }

                if (k > 0) continue;

                for (int m=k+1; m<nBodies; ++m)
                {
                    bnm = workingOps[m].bodyNum - 1;

                    int i2 = nBodies * k + m;
                    int action = zoneSpecs[i][i2];
                    if (action > -1)
                    {
                        vtkSmartPointer<vtkBooleanOperationPolyDataFilter> boolFilter =
                                    vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();

                        boolFilter->SetOperation(action);
                        if (march == -1)
                        {
                            boolFilter->SetInputConnection( 0, (srcs)[bnk+start]->GetOutputPort() );
                            boolFilter->SetInputConnection( 1, (srcs)[bnm+start]->GetOutputPort() );
                        }
                        else
                        {
                            boolFilter->SetInputConnection( 0, (bools)[march]->GetOutputPort() );
                            boolFilter->SetInputConnection( 1, (srcs)[bnm+start]->GetOutputPort() );
                        }
                        boolFilter->GlobalWarningDisplayOff();
                        boolFilter->Update();
                        ++march;
                        bools.push_back(boolFilter);
                    }
                }           //-- end m-loop
            }               //-- end k-loop

            if ((bools.size() > 0) &&  (maps.size() > 0))
            {
                vtkNew<vtkPolyData> map;
                map->ShallowCopy(bools[march]->GetOutput( ));
                maps.push_back(map);

                vtkNew<vtkAppendPolyData> appendFilter;
                for (int i=0; i<maps.size(); ++i)
                {
                    appendFilter->AddInputData(maps[i]);
                }

                // Remove any duplicate points.
                vtkNew<vtkCleanPolyData> cleanFilter;
                cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
                cleanFilter->Update();

                normals1->SetInputConnection(cleanFilter->GetOutputPort( 0 ));
            }
            else if (bools.size() > 0)
            {
                normals1->SetInputConnection(bools[march]->GetOutputPort( 0 ));
            }

            else if (maps.size() > 0)
            {
                vtkNew<vtkAppendPolyData> appendFilter;
                for (int i=0; i<maps.size(); ++i)
                {
                    appendFilter->AddInputData(maps[i]);
                }

                // Remove any duplicate points.
                vtkNew<vtkCleanPolyData> cleanFilter;
                cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
                cleanFilter->Update();

                normals1->SetInputConnection(cleanFilter->GetOutputPort( 0 ));
            }
        }                       //-- end else

        vtkNew<vtkDecimatePro> deci;
        deci->SetInputConnection(normals1->GetOutputPort());
        if ((*cgZoneOpsLocal)[i].meshReduction > 0)
        {
            double r = (*cgZoneOpsLocal)[i].meshReduction * 0.01;
            deci->SetTargetReduction(r);
        }
        else
            deci->SetTargetReduction(0.0);

        mapper->SetInputConnection(deci->GetOutputPort());
        mapper->ScalarVisibilityOff();
        mapper->GetBounds(b);

        colorR.push_back(color.redF());
        colorG.push_back(color.greenF());
        colorB.push_back(color.blueF());

        zoneDisplay.push_back(display);
        zoneMappers.push_back(mapper);
        start += nBodies;
    }                           //-- end i-loop
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::showOneZone(QVector<CGBodyBasics>   *iNcgBodyBasics,
                                QVector<CGZoneOps>      *iNcgZoneOps,
                                std::vector<double> *redF,
                                std::vector<double> *greF,
                                std::vector<double> *bluF,
                                Bounds *inBounds)
{
    cgBodyBasics = iNcgBodyBasics;
    cgZoneOps    = iNcgZoneOps;
    mBounds      = inBounds;
    mBounds->reset();

    colorR.clear();
    colorG.clear();
    colorB.clear();
    for (auto i=0; i < redF->size(); ++i)
    {
        colorR.push_back( (*redF)[i] );
        colorG.push_back( (*greF)[i] );
        colorB.push_back( (*bluF)[i] );
    }
    clearAllActors();
    makeAxisTriad();
    makeCameraOrientationWidget();

    if (noBooleanOps)
        makeZoneMappersNoBoolean(iNcgZoneOps);
    else
        makeZoneMappersBoolean(iNcgZoneOps);

    makeZoneActors();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::updateClippingSurfaces(const int axes, int dir, double plane)
{
    if (axes == 1)
    {
        xpt[0] = plane;
        if (dir == 1)
            xnorm[0] = -1.0;
        else
            xnorm[0] = 1.0;
    }
    else if (axes == 2)
    {
        ypt[1] = plane;
        if (dir == 1)
            ynorm[1] = -1.0;
        else
            ynorm[1] = 1.0;
    }
    else if (axes == 3)
    {
        zpt[2] = plane;
        if (dir == 1)
            znorm[2] = -1.0;
        else
            znorm[2] = 1.0;
    }

    clearAllActors();
    makeAxisTriad();
    makeCameraOrientationWidget();
    makeZoneActors();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::updateClippingSurfaces2(const int axes, int dir, double plane)
{
    if (axes == 1)
    {
        xpt[0] = plane;
        if (dir == 1)
            xnorm[0] = -1.0;
        else
            xnorm[0] = 1.0;
    }
    else if (axes == 2)
    {
        ypt[1] = plane;
        if (dir == 1)
            ynorm[1] = -1.0;
        else
            ynorm[1] = 1.0;
    }
    else if (axes == 3)
    {
        zpt[2] = plane;
        if (dir == 1)
            znorm[2] = -1.0;
        else
            znorm[2] = 1.0;
    }

    clearAllActors();
    makeAxisTriad();
    makeCameraOrientationWidget();
    if (viewMode == 0)
        makeZoneActors2();
    else
        makeZoneActorsWithGrid();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneMappersNoBoolean(QVector<CGZoneOps>  *cgZoneOpsLocal)
{
    zoneMappers.clear();

    // --------------------------------------------------------------
    // Set up clip planes
    auto clipPlanes = vtkSmartPointer<vtkPlanes>::New();

    setupClipPlanes(clipPlanes);

    // --------------------------------------------------------------
    // Call to makeOneZone -- returning mapper or passing mapper?
    int i, k;

    srcs.clear();

    for(i=0, k=0; i < (int) cgZoneOpsLocal->size(); ++i)
    {
        if ((*cgZoneOpsLocal)[i].display)
        {
            int j = (*cgZoneOpsLocal)[i].bodyNum - 1;
            makeBody( (*cgBodyBasics)[j] );

            vtkNew<vtkPolyDataNormals> normals1;
            normals1->SetInputConnection((srcs)[k]->GetOutputPort());

            vtkNew<vtkDecimatePro> deci;
            deci->SetInputConnection(normals1->GetOutputPort());
            if ((*cgZoneOpsLocal)[i].meshReduction > 0)
            {
                double r = (*cgZoneOpsLocal)[i].meshReduction * 0.01;
                deci->SetTargetReduction(r);
            }
            else
                deci->SetTargetReduction(0.0);

            vtkNew<vtkOpenGLPolyDataMapper> zoneMapper;
            zoneMapper->SetInputConnection( deci->GetOutputPort() );
            zoneMapper->SetClippingPlanes(clipPlanes);
            zoneMapper->ScalarVisibilityOff();

            zoneDisplay.push_back(true);
            zoneMappers.push_back(zoneMapper);
            ++k;
        }
    }
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneMappersBoolean(QVector<CGZoneOps>  *cgZoneOpsLocal)
{
    zoneMappers.clear();
    zoneDisplay.clear();

    setupClipPlanes();
    // --------------------------------------------------------------
    /*  CGZoneOps
     *
        int boolOp;    0 - AND; 1 - OR
        int senseOp;  -1 - IN;  1 - OUT
        int bodyNum;
        bool display;
    */

    checkCellBooleanOperations2(cgZoneOpsLocal);

    std::vector < vtkSmartPointer < vtkPolyData >> maps;

    int start = 0;
    for (int i=0; i<numCodeZones[0]; ++i)
    {
        vtkNew<vtkPolyDataNormals> normals1;
        vtkNew<vtkOpenGLPolyDataMapper> mapper;

        int nBodies = codeZoneOr[0][i];

        maps.clear();
        if (nBodies == 1)
        {
            normals1->SetInputConnection((srcs)[start]->GetOutputPort( ));
        }
        else
        {
            int march = -1;
            bools.clear();
            for (int k=0; k<nBodies; ++k)
            {
                int i1;
                i1 = nBodies *  k + k;
                if (zoneSpecs[i][i1] == 0)
                {
                    // Process body that has no interaction with the rest.
                    vtkNew<vtkPolyData> map;
                    map->ShallowCopy((srcs)[k]->GetOutput( ));
                    maps.push_back(map);
                    continue;
                }

                // if (k > 0) continue;

                for (int m=k+1; m<nBodies; ++m)
                {
                    int i2 = nBodies * k + m;
                    int action = zoneSpecs[i][i2];
                    if (action > -1)
                    {
                        vtkSmartPointer<vtkBooleanOperationPolyDataFilter> boolFilter =
                                    vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();

                        boolFilter->SetOperation(action);
                        if (march == -1)
                        {
                            boolFilter->SetInputConnection( 0, (srcs)[k+start]->GetOutputPort() );
                            boolFilter->SetInputConnection( 1, (srcs)[m+start]->GetOutputPort() );
                        }
                        else
                        {
                            boolFilter->SetInputConnection( 0, (bools)[march]->GetOutputPort() );
                            boolFilter->SetInputConnection( 1, (srcs)[m+start]->GetOutputPort() );
                        }
                        boolFilter->GlobalWarningDisplayOff();
                        boolFilter->Update();
                        ++march;
                        bools.push_back(boolFilter);
                    }
                }           //-- end m-loop
            }               //-- end k-loop

            if ((bools.size() > 0) &&  (maps.size() > 0))
            {
                vtkNew<vtkPolyData> map;
                map->ShallowCopy(bools[march]->GetOutput( ));
                maps.push_back(map);

                vtkNew<vtkAppendPolyData> appendFilter;
                for (int i=0; i<maps.size(); ++i)
                {
                    appendFilter->AddInputData(maps[i]);
                }

                // Remove any duplicate points.
                vtkNew<vtkCleanPolyData> cleanFilter;
                cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
                cleanFilter->Update();

                normals1->SetInputConnection(cleanFilter->GetOutputPort( 0 ));
            }
            else if (bools.size() > 0)
            {
                normals1->SetInputConnection(bools[march]->GetOutputPort( 0 ));
            }

            else if (maps.size() > 0)
            {
                vtkNew<vtkAppendPolyData> appendFilter;
                for (int i=0; i<maps.size(); ++i)
                {
                    appendFilter->AddInputData(maps[i]);
                }

                // Remove any duplicate points.
                vtkNew<vtkCleanPolyData> cleanFilter;
                cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
                cleanFilter->Update();

                normals1->SetInputConnection(cleanFilter->GetOutputPort( 0 ));
            }
        }                       //-- end else

        vtkNew<vtkDecimatePro> deci;
        deci->SetInputConnection(normals1->GetOutputPort());
        if ((*cgZoneOpsLocal)[i].meshReduction > 0)
        {
            double r = (*cgZoneOpsLocal)[i].meshReduction * 0.01;
            deci->SetTargetReduction(r);
        }
        else
            deci->SetTargetReduction(0.0);

        mapper->SetInputConnection(deci->GetOutputPort());
        mapper->ScalarVisibilityOff();

        zoneDisplay.push_back(true);
        zoneMappers.push_back(mapper);
        start += nBodies;

    }                           //-- end i-loop
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::checkCellBooleanOperations2(QVector<CGZoneOps>  *cgZoneOpsLocal)
{
    int i1;
    int k1;
    int action;

    std::vector<int> or_info;
    std::vector<int> in_info;
    codeZoneIn.clear();
    codeZoneOr.clear();
    numCodeZones.clear();
    zoneSpecs.clear();

    i1 = 0;
    for (int i=0; i<cgZoneOpsLocal->size(); ++i)
    {
        or_info.push_back(i1);
        in_info.push_back(i1);
    }

    // Ensure that 1st / leading operation is an AND and not an OR.
    if ((*cgZoneOpsLocal)[0].boolOp == 1)
        (*cgZoneOpsLocal)[0].boolOp = 0;

    // Find the number of bodies in each code zone.
    int nCodeZones = 0;
    for (int i=0; i<cgZoneOpsLocal->size(); ++i)
    {
        if ( (*cgZoneOpsLocal)[i].boolOp == 1)
        {
            ++nCodeZones;
            ++or_info[nCodeZones];
        }
        else
            ++or_info[nCodeZones];

        // Find the number inside.
        if ((*cgZoneOpsLocal)[i].senseOp < 0)
            ++in_info[nCodeZones];
    }
    ++nCodeZones;

    numCodeZones.push_back(nCodeZones);
    codeZoneIn.push_back(in_info);
    codeZoneOr.push_back(or_info);

    QVector<CGZoneOps> workingOps;

    srcs.clear();
    std::vector<int> check;

    int start = 0;
    for (int i=0; i<nCodeZones; ++i)
    {
        workingOps.clear();
        // Grab the data for the code zone.
        for(int k=start; k < (or_info[i] + start); ++k)
        {
            workingOps.push_back( (*cgZoneOpsLocal)[k] );
        }
        // Sort in order by body number with its sense.
        std::sort(workingOps.begin(), workingOps.end());

        // Make the bodies and store in "srcs".
        for(int k=0; k < (or_info[i]); ++k)
        {
            int j = workingOps[k].bodyNum - 1;
            makeBody( (*cgBodyBasics)[j] );
        }

        check.clear();

        // Only need to check boolean operations if there is more than 1 body.
        if (or_info[i] > 1)
        {
            int n = or_info[i];

            // Initialize 1-D interaction matrix.
            for(int k=0; k < (n*n) ; ++k) check.push_back(-1);
            for(int k=0; k < n; ++k)
            {
                i1 = k*n + k;
                check[i1] = 0;
            }

            for (int k=0; k<workingOps.size(); ++k)
            {
                for (int m=k+1; m<workingOps.size(); ++m)
                {
                    vtkSmartPointer<sssBooleanOperationCheck> boolFilter =
                        vtkSmartPointer<sssBooleanOperationCheck>::New();

                    if ((workingOps[k].senseOp + workingOps[m].senseOp) == 0)
                    {
                        if (workingOps[k].senseOp == -1)
                        {
                            i1 = 0;
                            k1 = 1;
                        }
                        else
                        {
                            i1 = 1;
                            k1 = 0;
                        }
                        action = sssBooleanOperationCheck::VTK_DIFFERENCE;
                    }
                    else if ((workingOps[k].senseOp + workingOps[m].senseOp) == 2)
                    {
                        i1 = 0;
                        k1 = 1;
                        action = sssBooleanOperationCheck::VTK_UNION;
                    }
                    else
                    {
                        i1 = 0;
                        k1 = 1;
                        action = sssBooleanOperationCheck::VTK_INTERSECTION;
                    }

                    boolFilter->SetOperation( action );
                    boolFilter->SetInputConnection( i1, (srcs)[k]->GetOutputPort() );
                    boolFilter->SetInputConnection( k1, (srcs)[m]->GetOutputPort() );

                    boolFilter->GlobalWarningDisplayOff();
                    bool b1 = false;
                    b1 = boolFilter->GetExecutive()->Update();
                    if (b1)
                    {
                        i1 = k * n + k;
                        ++check[i1];
                        i1 += (m - k);

                        check[i1] = action;
                        i1 = m * n + m;

                        ++check[i1];
                        i1 = m * n + k;

                        check[i1] = action;
                    }
                }               // -- end m-loop
            }                   // -- end k-loop

            // Some input checks for sense.
            for(int k=0; k<n; ++k)
            {
                i1 = k*n + k;
                if ((check[i1] == 0) && (or_info[i] > 1))
                {
                    if (workingOps[k].senseOp == -1)
                    {
                        if (in_info[i] > 1)
                        {
                            userWarning(0, workingOps[k].bodyNum);
                        }
                    }
                    else if (workingOps[k].senseOp == 1)
                    {
                        if (in_info[i] == 0)
                        {
                            userWarning(1, workingOps[k].bodyNum);
                        }
                    }
                }               // -- end if check[i1] == 0
            }                   // -- end k-llop
        }                       // -- or_info > 1
        else
        {
            check.push_back(0);
        }

        // Save interaction matrix here.
        zoneSpecs.push_back(check);

        // Increment for next code zone.
        start += or_info[i];
    }                           // -- end i-loop over code zones
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::userWarning(const int code, const int bodyNum)
{
    QString s1;
    QString numStr;
    numStr.setNum(bodyNum);

    QMessageBox msg;

    if (code == 0)
    {
        s1  = "Body sense may not be correct for body: " + numStr + "\n\n";
        s1 += "Sense is inside, but this body does not intersect other bodies ";
        s1 += "some of which have inside sense.\n\n";
        s1 += "This body will be renedered, but may not be what is intended.\n";
    }
    else if (code == 1)
    {
        s1  = "Body sense may not be correct for body: " + numStr + "\n\n";
        s1 += "Sense is outside, but this body does not intersect other bodies ";
        s1 += "some of which have outside sense.\n\n";
        s1 += "This body will be renedered, but may not be what is intended.\n";
    }

    msg.setIcon(QMessageBox::Warning);
    msg.setInformativeText(s1);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.show();
    msg.exec();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::makeBody(CGBodyBasics &body)
{
    vtkAlgorithmOutput *va;
    std::vector<double> *p;

    int bt = body.bodyType;
    p = &body.parameters;

    switch(bt)
    {
        case(SPH):
        {
            vtkNew<vtkSphereSource> sphere1;
            sphere1->SetCenter((*p)[0], (*p)[1], (*p)[2]);
            sphere1->SetRadius((*p)[3]);
            sphere1->SetPhiResolution(60);
            sphere1->SetThetaResolution(60);
            srcs.push_back(sphere1);
            break;
        }
        case(ELL):
        {
            vtkNew<sssEllipsoidSource> ell1;
            ell1->SetEllipsoid(sssEllipsoidSource::VTK_ELL);
            double loci[6];
            loci[0] = (*p)[0];
            loci[1] = (*p)[1];
            loci[2] = (*p)[2];
            loci[3] = (*p)[3];
            loci[4] = (*p)[4];
            loci[5] = (*p)[5];
            ell1->SetLoci(loci);
            ell1->SetRadius((*p)[6]);
            ell1->SetThetaResolution(60);
            ell1->SetPhiResolution(60);
            srcs.push_back(ell1);
            break;
        }
        case(ELR):
        {
            vtkNew<sssEllipsoidSource> elr1;
            elr1->SetEllipsoid(sssEllipsoidSource::VTK_ELR);
            double vec[3];
            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            elr1->SetHeight(vec);

            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            elr1->SetCenter(vec);

            elr1->SetRadius((*p)[6]);
            elr1->SetThetaResolution(60);
            elr1->SetPhiResolution(60);
            srcs.push_back(elr1);
            break;
        }
        case(RCC):
        {
            vtkNew<sssSuperCylinderSource> rcc1;
            rcc1->SetSuperCylinder(sssSuperCylinderSource::VTK_RCC);
            double vec[3];
            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            rcc1->SetHVector(vec);

            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            rcc1->SetAnchor(vec);

            rcc1->SetRadiusBottom((*p)[6]);
            rcc1->SetResHeight(30);
            rcc1->SetResRadius(60);
            srcs.push_back(rcc1);
            break;
        }
        case(REC):
        {
            vtkNew<sssSuperCylinderSource> rcc1;
            rcc1->SetSuperCylinder(sssSuperCylinderSource::VTK_REC);
            double vec[3];
            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            rcc1->SetHVector(vec);

            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            rcc1->SetAnchor(vec);

            vec[0] = (*p)[6];
            vec[1] = (*p)[7];
            vec[2] = (*p)[8];
            rcc1->SetRMajor(vec);

            vec[0] = (*p)[9];
            vec[1] = (*p)[10];
            vec[2] = (*p)[11];
            rcc1->SetRMinor(vec);

            rcc1->SetResHeight(30);
            rcc1->SetResRadius(60);
            srcs.push_back(rcc1);
            break;
        }
        case(TRC):
        {
            vtkNew<sssSuperCylinderSource> rcc1;
            rcc1->SetSuperCylinder(sssSuperCylinderSource::VTK_TRC);
            double vec[3];
            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            rcc1->SetHVector(vec);

            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            rcc1->SetAnchor(vec);

            rcc1->SetRadiusBottom((*p)[6]);
            rcc1->SetRadiusTop((*p)[7]);

            rcc1->SetResHeight(30);
            rcc1->SetResRadius(60);
            srcs.push_back(rcc1);
            break;
        }
        case(RPP):
        {
            vtkNew<sssSuperCubeSource> rpp1;
            rpp1->SetSuperCube(sssSuperCubeSource::VTK_RPP);
            double vec[3];
            vec[0] = (*p)[0];
            vec[1] = (*p)[2];
            vec[2] = (*p)[4];
            rpp1->SetAnchor(vec);

            vec[0] = (*p)[1] - (*p)[0];
            vec[1] = 0.0;
            vec[2] = 0.0;
            rpp1->SetA1(vec);

            vec[0] = 0.0;
            vec[1] = (*p)[3] - (*p)[2];
            vec[2] = 0.0;
            rpp1->SetA2(vec);

            vec[0] = 0.0;
            vec[1] = 0.0;
            vec[2] = (*p)[5] - (*p)[4];
            rpp1->SetA3(vec);

            rpp1->SetResA1(30);
            rpp1->SetResA1(30);
            rpp1->SetResA1(30);

            srcs.push_back(rpp1);
            break;
        }
        case(BOX):
        {
            vtkNew<sssSuperCubeSource> box1;
            box1->SetSuperCube(sssSuperCubeSource::VTK_BOX);
            double vec[3];
            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            box1->SetAnchor(vec);

            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            box1->SetA1(vec);

            vec[0] = (*p)[6];
            vec[1] = (*p)[7];
            vec[2] = (*p)[8];
            box1->SetA2(vec);

            vec[0] = (*p)[9];
            vec[1] = (*p)[10];
            vec[2] = (*p)[11];
            box1->SetA3(vec);

            box1->SetResA1(30);
            box1->SetResA1(30);
            box1->SetResA1(30);

            srcs.push_back(box1);
            break;
        }
        case(WED):
        {
            vtkNew<sssSuperCubeSource> box1;
            box1->SetSuperCube(sssSuperCubeSource::VTK_WEDGE);
            double vec[3];
            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            box1->SetAnchor(vec);

            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            box1->SetA1(vec);

            vec[0] = (*p)[6];
            vec[1] = (*p)[7];
            vec[2] = (*p)[8];
            box1->SetA2(vec);

            vec[0] = (*p)[9];
            vec[1] = (*p)[10];
            vec[2] = (*p)[11];
            box1->SetA3(vec);

            box1->SetResA1(30);
            box1->SetResA1(30);
            box1->SetResA1(30);

            srcs.push_back(box1);
            break;
        }
        case(TOR):
        {
            vtkNew<sssTorusSource> tor1;
            double vec[3];
            vec[0] = (*p)[0];
            vec[1] = (*p)[1];
            vec[2] = (*p)[2];
            tor1->SetCenter(vec);

            vec[0] = (*p)[3];
            vec[1] = (*p)[4];
            vec[2] = (*p)[5];
            tor1->SetHVector(vec);

            tor1->SetRadius((*p)[6]);
            tor1->SetHRadius((*p)[7]);
            tor1->SetPRadius((*p)[8]);

            tor1->SetSegmentsAboutAxis(40);
            tor1->SetSegmentsOnSurface(40);

            srcs.push_back(tor1);
            break;
        }
    }           // -- end switch
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneActors()
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto clipPlanes = vtkSmartPointer<vtkPlanes>::New();

    setupClipPlanes(clipPlanes);
    addCGZoneActors(clipPlanes);

    // --------------------------------------------------------------
    finalizeRenderWindow();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneActors2()
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto clipPlanes = vtkSmartPointer<vtkPlanes>::New();

    setupClipPlanes(clipPlanes);
    addCGZoneActors(clipPlanes);
    addUGridZoneActors(clipPlanes);

    // --------------------------------------------------------------
    finalizeRenderWindow();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::finalizeRenderWindow()
{
    ren->ResetCamera();
    ren->SetBackground(0.98, 0.98, 0.98);

    renderWindow->AddRenderer(ren);

    iRen = renderWindow->GetInteractor();

    renderWindow->Render();

    iRen->Initialize();
    renderWindow->Render();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::setNoBooleanOp(bool state)
{
    noBooleanOps = state;
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::showMeshAssembly(ReadFileAbaqus *abaqus, Bounds *inBounds,
                                     int universe)
{
    // This is to show a mesh assembly with its background cell.
    // Assume inCgZoneOps vector has only 1 entry.

    mBounds      = inBounds;
    QVector<CGBodyBasics>   bBasics;
    md->makeBodyBasics(&bBasics);
    cgBodyBasics = &bBasics;

    QVector<CellData> cData;
    md->makeCellList(&cData);

    QStringList sl = md->universeDataStringList[universe];

    int cell = sl[3].toInt();  // -- use fill cell number

    int cIndex = md->getCellNumberIndex(cell);

    clearAllActors();
    makeAxisTriad();
    makeCameraOrientationWidget();

    viewMode = 1;

    colorR.clear();
    colorG.clear();
    colorB.clear();

    colorR.push_back( cData[cIndex].color.redF() );
    colorG.push_back( cData[cIndex].color.greenF() );
    colorB.push_back( cData[cIndex].color.blueF() );

    md->getInstanceDataColors(&ugridColorR, &ugridColorG, &ugridColorB);

    zoneMappers.clear();
    ugrids.clear();
    ugridDisplay.clear();
    ugridMappers.clear();

    makeUMeshModel(abaqus);
    makeUMeshMappers();
    makeZoneMappersBoolean(&cData[cIndex].zoneOps);

    makeZoneActorsWithGrid();
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeUMeshModel(ReadFileAbaqus *abaqus)
{
    double tmp[6];

    md->getMeshModParameters(&abaqus->abaqusFile, &tmp[0]);

    double scale     = tmp[0];
    double transx    = tmp[1];
    double transy    = tmp[2];
    double transz    = tmp[3];
    double azimuthal = tmp[4];
    double polar     = tmp[5];

    double x[3];

    int univ = md->getMeshUniverseNumber(&abaqus->abaqusFile);

    bool flag;
    int  cell;

    for(int i=0; i<abaqus->nInstances; ++i)
    {
//        flag  = abaqus->instances[i].display;
        cell = md->getInstanceCellNumberByIndex(univ, i);
        flag = md->getInstanceDisplay(univ, cell);
        ugridDisplay.push_back(flag);

        int nEls = 0;
        for(int j=0; j<6; ++j)
            nEls += abaqus->instances[i].elNumbers[j];

        vtkNew<vtkUnstructuredGrid> ugrid;
        ugrid->Allocate( nEls );

        int nH1 = abaqus->instances[i].h1elements.size();
        int nP1 = abaqus->instances[i].p1elements.size();
        int nT1 = abaqus->instances[i].t1elements.size();

        vtkIdType indices[8];
        for(int j=0; j<nH1; ++j)
        {
            for(int k=0; k < 8; ++k)
            {
                indices[k] = abaqus->instances[i].h1elements[j].connect[k];
            }
            ugrid->InsertNextCell(VTK_HEXAHEDRON, 8, indices);

        }                   // -- end j-loop

        for(int j=0; j<nT1; ++j)
        {
            for(int k=0; k < 4; ++k)
            {
                indices[k] = abaqus->instances[i].t1elements[j].connect[k];
            }
            ugrid->InsertNextCell(VTK_TETRA, 4, indices);

        }                   // -- end j-loop

        for(int j=0; j<nP1; ++j)
        {
            for(int k=0; k < 6; ++k)
            {
                indices[k] = abaqus->instances[i].p1elements[j].connect[k];
            }
            ugrid->InsertNextCell(VTK_WEDGE, 6, indices);

        }                   // -- end j-loop

        vtkNew<vtkPoints> points;
        int nNodes = abaqus->instances[i].nodes.size();
        for(int n=0; n<nNodes; ++n)
        {
            x[0] = abaqus->instances[i].nodes[n].x;
            x[1] = abaqus->instances[i].nodes[n].y;
            x[2] = abaqus->instances[i].nodes[n].z;
            rotatePoint(x[0], x[1], x[2], azimuthal, polar);
            x[0] = x[0] * scale + transx;
            x[1] = x[1] * scale + transy;
            x[2] = x[2] * scale + transz;

            points->InsertPoint(n, x);
        }
        ugrid->SetPoints(points);

        ugrids.push_back(ugrid);

    }                       // -- end i-loop
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::makeUMeshMappers()
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto clipPlanes = vtkSmartPointer<vtkPlanes>::New();

    setupClipPlanes(clipPlanes);

    for(int u=0; u < ugrids.size(); ++u)
    {
        vtkNew<vtkDataSetMapper> ugridMapper;
        ugridMapper->SetClippingPlanes(clipPlanes);
        ugridMapper->SetInputData(ugrids[u]);
        ugridMappers.push_back(ugridMapper);
    }
}
// ----------------------------------------------------------------------------
void BaseVtkWindow::makeZoneActorsWithGrid()
{
    // --------------------------------------------------------------
    // Set up clip planes
    auto clipPlanes = vtkSmartPointer<vtkPlanes>::New();

    setupClipPlanes(clipPlanes);

    addUGridZoneActors(clipPlanes);

    addCGZoneActors(clipPlanes);

    // --------------------------------------------------------------
    finalizeRenderWindow();
}

// ----------------------------------------------------------------------------
void BaseVtkWindow::addUGridZoneActors(vtkSmartPointer<vtkPlanes> clipPlanes)
{
    double ambient = zoneProperty1->GetAmbient();
    double diffuse = zoneProperty1->GetDiffuse();

    double bounds[6];
    double color[3];

    int i;
    for(i=0; i < (int) ugridMappers.size(); ++i)
    {
        if (ugridDisplay[i])
        {
            ugridMappers[i]->SetClippingPlanes(clipPlanes);

            vtkNew<vtkActor> zoneActor1;
            zoneActor1->GetProperty()->SetColor(ugridColorR[i], ugridColorG[i], ugridColorB[i]);
            zoneActor1->GetProperty()->SetAmbient(ambient);
            zoneActor1->GetProperty()->SetDiffuse(diffuse);
            zoneActor1->GetProperty()->SetInterpolationToPhong();
            zoneActor1->GetProperty()->ShadingOn();
            zoneActor1->GetProperty()->SetOpacity(transparancy);
            zoneActor1->GetProperty()->EdgeVisibilityOn();

            color[0] = ugridColorR[i];
            color[1] = ugridColorG[i];
            color[2] = ugridColorB[i];

            if (backFaceStyle > 0)
            {
                color[0] = (1.0 - whiteWashFactor) * color[0] + whiteWashFactor;
                color[1] = (1.0 - whiteWashFactor) * color[1] + whiteWashFactor;
                color[2] = (1.0 - whiteWashFactor) * color[2] + whiteWashFactor;
            }

            vtkNew<vtkProperty> bProperty;
            zoneActor1->SetBackfaceProperty(bProperty);
            zoneActor1->GetBackfaceProperty()->SetColor(color);
            zoneActor1->GetBackfaceProperty()->SetAmbient(ambient);
            zoneActor1->GetBackfaceProperty()->SetDiffuse(diffuse);

            ugridMappers[i]->GetBounds(bounds);
            updateBounds(bounds);

            zoneActor1->SetMapper( ugridMappers[i] );
            ren->AddActor( zoneActor1 );
        }
    }
}


// ----------------------------------------------------------------------------
void BaseVtkWindow::addCGZoneActors(vtkSmartPointer<vtkPlanes> clipPlanes)
{
    double color[3];
    double ambient = zoneProperty1->GetAmbient();
    double diffuse = zoneProperty1->GetDiffuse();

    double bounds[6];

    for(auto i=0; i < zoneMappers.size(); ++i)
    {
        if (zoneDisplay[i])
        {
            zoneMappers[i]->SetClippingPlanes(clipPlanes);

            zoneMappers[i]->GetBounds(bounds);
            updateBounds(bounds);

            vtkNew<vtkActor> zoneActor1;
            zoneActor1->GetProperty()->SetColor(colorR[i], colorG[i], colorB[i]);
            zoneActor1->GetProperty()->SetAmbient(ambient);
            zoneActor1->GetProperty()->SetDiffuse(diffuse);
            zoneActor1->GetProperty()->SetInterpolationToPhong();
            zoneActor1->GetProperty()->ShadingOn();
            zoneActor1->GetProperty()->SetOpacity(transparancy);

            color[0] = colorR[i];
            color[1] = colorG[i];
            color[2] = colorB[i];

            if (backFaceStyle > 0)
            {
                color[0] = (1.0 - whiteWashFactor) * color[0] + whiteWashFactor;
                color[1] = (1.0 - whiteWashFactor) * color[1] + whiteWashFactor;
                color[2] = (1.0 - whiteWashFactor) * color[2] + whiteWashFactor;
            }

            vtkNew<vtkProperty> bProperty;
            zoneActor1->SetBackfaceProperty(bProperty);
            zoneActor1->GetBackfaceProperty()->SetColor(color);
            zoneActor1->GetBackfaceProperty()->SetAmbient(ambient);
            zoneActor1->GetBackfaceProperty()->SetDiffuse(diffuse);

            zoneActor1->SetMapper( zoneMappers[i] );
            ren->AddActor( zoneActor1 );
        }
    }
}                   // -- addCGZoneActors

// ----------------------------------------------------------------------------
void BaseVtkWindow::updateBounds(double *bounds)
{
    mBounds->nodesMinX = (bounds[0] < mBounds->nodesMinX ) ? bounds[0] : mBounds->nodesMinX ;
    mBounds->nodesMinY = (bounds[2] < mBounds->nodesMinY ) ? bounds[2] : mBounds->nodesMinY ;
    mBounds->nodesMinZ = (bounds[4] < mBounds->nodesMinZ ) ? bounds[4] : mBounds->nodesMinZ ;

    mBounds->nodesMaxX = (bounds[1] > mBounds->nodesMaxX ) ? bounds[1] : mBounds->nodesMaxX ;
    mBounds->nodesMaxY = (bounds[3] > mBounds->nodesMaxY ) ? bounds[3] : mBounds->nodesMaxY ;
    mBounds->nodesMaxZ = (bounds[5] > mBounds->nodesMaxZ ) ? bounds[5] : mBounds->nodesMaxZ ;
}
