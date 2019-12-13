
/**
 * @brief - Reimplementation of a program started in 04/2010 as a
 *          training and a tool to visualize fractals of various
 *          types. This is a good first test scenario for the new
 *          engine framework as it is quite a complete application.
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/LoggerLocator.hh>

# include <sdl_app_core/SdlApplication.hh>
# include <core_utils/CoreException.hh>
# include <sdl_core/SdlWidget.hh>
# include "FractalRenderer.hh"
# include "MandelbrotOptions.hh"
# include "JuliaOptions.hh"
# include "NewtonOptions.hh"
# include "RenderingStatus.hh"
# include "StatusBar.hh"

// TODO: Handle scrolling.
// TODO: Allow customization of Palette (probably through the menu bar).
// TODO: Crash when zooming too fast: it comes from `computeCellFromCoords` where apparently
// some cell can't compute the zoom when it changes too fast.
// TODO: Check rounding error at high zoom levels.
// TODO: Crash when using textbox (removing text for example).
// TODO: Introduce CUDA ? (like here: https://cmake.org/cmake/help/v3.12/module/FindCUDA.html)
// Or here (https://devblogs.nvidia.com/building-cuda-applications-cmake/)

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger logger;
  utils::LoggerLocator::provide(&logger);

  const std::string service("fractsim");
  const std::string module("main");

  // Create the application window parameters.
  const std::string appName = std::string("fractsim");
  const std::string appTitle = std::string("The best way to get wallpapers (c)");
  const std::string appIcon = std::string("data/img/icon.bmp");
  const utils::Sizei size(800, 600);

  sdl::app::SdlApplicationShPtr app = nullptr;

  try {
    app = std::make_shared<sdl::app::SdlApplication>(
      appName,
      appTitle,
      appIcon,
      size,
      true,
      utils::Sizef(0.4f, 0.6f),
      50.0f,
      60.0f
    );

    // Create the layout of the window: the main tab is a scrollable widget
    // allowing the display of the fractal. The rigth dock widget allows to
    // control the computation parameters of the fractal.
    fractsim::FractalRenderer* renderer = new fractsim::FractalRenderer();
    app->setCentralWidget(renderer);

    fractsim::MandelbrotOptions* mandelOpt = new fractsim::MandelbrotOptions();
    app->addDockWidget(mandelOpt, sdl::app::DockWidgetArea::RightArea, std::string("Mandelbrot"));

    fractsim::JuliaOptions* juliaOpt = new fractsim::JuliaOptions();
    app->addDockWidget(juliaOpt, sdl::app::DockWidgetArea::RightArea, std::string("Julia"));

    fractsim::NewtonOptions* newtonOpt = new fractsim::NewtonOptions(5u);
    app->addDockWidget(newtonOpt, sdl::app::DockWidgetArea::RightArea, std::string("Newton"));

    fractsim::RenderingStatus* status = new fractsim::RenderingStatus();
    app->addDockWidget(status, sdl::app::DockWidgetArea::TopArea);

    fractsim::StatusBar* bar = new fractsim::StatusBar();
    app->setStatusBar(bar);

    // Connect the options changed signal to the request rendering slot.
    mandelOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );
    juliaOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );
    newtonOpt->onOptionsChanged.connect_member<fractsim::FractalRenderer>(
      renderer,
      &fractsim::FractalRenderer::requestRendering
    );

    // Connect the render button to the options panel slots.
    status->getRenderButton().onClick.connect_member<fractsim::MandelbrotOptions>(
      mandelOpt,
      &fractsim::MandelbrotOptions::validateOptions
    );
    status->getRenderButton().onClick.connect_member<fractsim::JuliaOptions>(
      juliaOpt,
      &fractsim::JuliaOptions::validateOptions
    );
    status->getRenderButton().onClick.connect_member<fractsim::NewtonOptions>(
      newtonOpt,
      &fractsim::NewtonOptions::validateOptions
    );

    status->getResetButton().onClick.connect_member<fractsim::MandelbrotOptions>(
      mandelOpt,
      &fractsim::MandelbrotOptions::resetOptions
    );
    status->getResetButton().onClick.connect_member<fractsim::JuliaOptions>(
      juliaOpt,
      &fractsim::JuliaOptions::resetOptions
    );
    status->getResetButton().onClick.connect_member<fractsim::NewtonOptions>(
      newtonOpt,
      &fractsim::NewtonOptions::resetOptions
    );

    renderer->onTileCompleted.connect_member<fractsim::RenderingStatus>(
      status,
      &fractsim::RenderingStatus::onCompletionChanged
    );

    renderer->onCoordChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onMouseCoordsChanged
    );
    renderer->onZoomChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onZoomChanged
    );
    renderer->onRenderingAreaChanged.connect_member<fractsim::StatusBar>(
      bar,
      &fractsim::StatusBar::onRenderingAreaChanged
    );

    // Run it.
    app->run();
  }
  catch (const utils::CoreException& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught internal exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (const std::exception& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (...) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Unexpected error while setting up application"),
      module,
      service
    );
  }

  app.reset();

  // All is good.
  return EXIT_SUCCESS;
}

/*
    14/04 -> 25/05
    PROJET DE CREATEUR DE FRACTALES
            Objectifs :
        initiaux ;
        -Cr�er des fractales du type Z(n+1)=Z(n)�+c _________________ OK
        -Demander � l'utilisateur le point de d�part ________________ OK
        secondaires ;
        -G�rer les fractales du type Z(n+1)=sqrt(Z(n)+1) ____________ Non r�ussi
        -Pouvoir dessiner l'ensemble de MandelBrot __________________ OK
        -Calculer des fractales de Newton pour n'importe
         quel polyn�me de degr� 5, � coefficients
         complexes ou non ___________________________________________ OK
        -Pouvoir dessiner l'ensemble de MandelBrot jusqu'�
         la puissance que l'on veut__________________________________ OK
        -Tracer des attracteurs �tranges ____________________________ OK
*/

# ifdef OLD

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 630

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include "add_on.h"
#include "ecriture_x_y_n.h"
#include "calcul.h"
#include "fonctions_choix_type_fractale.h"

int main (int argc, char* argv[])
{
    SDL_Surface* ecran=LoadSDL();

    int evenement=1, i=0, fract_type=0, n=2, choix=0;

    Image complexe;
    complexe.image=SDL_CreateRGBSurface(SDL_HWSURFACE,1,1,32,0,0,0,0);
    SDL_FillRect(complexe.image,NULL,SDL_MapRGB(ecran->format,0,255,0));

    SDL_Event event;
    TTF_Font* policeMenu=NULL;
    policeMenu=TTF_OpenFont("Police/GoodTime.ttf",15);
    SDL_Color couleurMenu={0,255,0};

    Image barreMenu;
    barreMenu.image=SDL_CreateRGBSurface(SDL_HWSURFACE,LARGEUR_FENETRE,30,32,0,0,0,0);
    SDL_FillRect(barreMenu.image,NULL,SDL_MapRGB(ecran->format,192,192,192));
    barreMenu.positionImage.x=0;
    barreMenu.positionImage.y=0;

    Image repere;
    repere.image=SDL_CreateRGBSurface(SDL_HWSURFACE,LARGEUR_FENETRE,HAUTEUR_FENETRE-30,32,0,0,0,0);
    SDL_FillRect(repere.image,NULL,SDL_MapRGB(ecran->format,255,255,255));
    repere.positionImage.x=0;
    repere.positionImage.y=barreMenu.positionImage.y+barreMenu.image->h;

    Bouton NewFract;
    NewFract.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
    NewFract.fond.positionImage.x=10;
    NewFract.fond.positionImage.y=2;
    NewFract.ecriture.image=TTF_RenderText_Blended(policeMenu,"Nouvelle Fractale",couleurMenu);
    NewFract.ecriture.positionImage.x=NewFract.fond.positionImage.x+NewFract.fond.image->w/2-NewFract.ecriture.image->w/2;
    NewFract.ecriture.positionImage.y=NewFract.fond.positionImage.y+NewFract.fond.image->h/2-NewFract.ecriture.image->h/2;

    Bouton Calcul;
    Calcul.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
    Calcul.fond.positionImage.x=250;
    Calcul.fond.positionImage.y=2;
    Calcul.ecriture.image=TTF_RenderText_Blended(policeMenu,"Calcul",couleurMenu);
    Calcul.ecriture.positionImage.x=Calcul.fond.positionImage.x+Calcul.fond.image->w/2-Calcul.ecriture.image->w/2;
    Calcul.ecriture.positionImage.y=Calcul.fond.positionImage.y+Calcul.fond.image->h/2-Calcul.ecriture.image->h/2;


    Coefficient polynome_coefficient[11];

    for(i=0 ; i<11 ; i++)
    {
        Initialise_Coefficient(&polynome_coefficient[i]);
        polynome_coefficient[i].puissance=i;
    }
    Coefficient Settings;
    Coefficient degre;
    Initialise_Coefficient(&Settings);
    Initialise_Coefficient(&degre);

    Image coordonnees;
    coordonnees.image=SDL_CreateRGBSurface(SDL_HWSURFACE,105,28,32,0,0,0,0);
    SDL_FillRect(coordonnees.image,NULL,SDL_MapRGB(ecran->format,255,255,255));
    coordonnees.positionImage.x=690;
    coordonnees.positionImage.y=0;

    Window Window_Settings;
    Window_Settings.xMax=0;
    Window_Settings.xMin=0;
    Window_Settings.yMax=0;
    Window_Settings.xMin=0;
    Window_Settings.zoom=0;
    Window_Settings.deltaX=0;
    Window_Settings.deltaY=0;

    Bouton zoom;
    zoom.fond.image=SDL_LoadBMP("Images/zoom.BMP");
    zoom.fond.positionImage.x=490;
    zoom.fond.positionImage.y=15;
    zoom.ecriture.image=NULL;

    SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
    SDL_BlitSurface(barreMenu.image,NULL,ecran,&barreMenu.positionImage);
    SDL_BlitSurface(coordonnees.image,NULL,ecran,&coordonnees.positionImage);
    SDL_BlitSurface(zoom.fond.image,NULL,ecran,&zoom.fond.positionImage);

    while(evenement!=0)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                evenement=0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        evenement=0;
                        break;
                    case SDLK_l:
                        if(fract_type==1)
                        {
                            Window_Settings.xMax=1;
                            Window_Settings.xMin=-1;
                            Window_Settings.yMax=1;
                            Window_Settings.yMin=-1;
                            Window_Settings.deltaX=2;
                            Window_Settings.deltaY=2;
                        }
                        if(fract_type==2)
                        {
                            Window_Settings.xMax=2;
                            Window_Settings.xMin=-2;
                            Window_Settings.yMax=1.5;
                            Window_Settings.yMin=-1.5;
                            Window_Settings.deltaX=4;
                            Window_Settings.deltaY=3;
                        }
                        if(fract_type==3)
                        {
                            Window_Settings.xMax=0.6;
                            Window_Settings.xMin=-2;
                            Window_Settings.yMax=1;
                            Window_Settings.yMin=-1;
                            Window_Settings.deltaX=2.6;
                            Window_Settings.deltaY=2;
                        }
                        if(fract_type==4)
                        {
                            Window_Settings.xMax=8;
                            Window_Settings.xMin=-8;
                            Window_Settings.yMax=6;
                            Window_Settings.yMin=-6;
                            Window_Settings.deltaX=16;
                            Window_Settings.deltaY=12;
                        }
                        Window_Settings.zoom=0;
                        break;
                    case SDLK_KP0:
                        choix=0;
                        break;
                    case SDLK_KP1:
                        choix=1;
                        break;
                    case SDLK_KP2:
                        choix=2;
                        break;
                    case SDLK_KP3:
                        choix=3;
                        break;
                    case SDLK_KP4:
                        choix=4;
                        break;
                    case SDLK_KP5:
                        choix=5;
                        break;
                    case SDLK_KP6:
                        choix=6;
                        break;
                    case SDLK_RETURN:
                        SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
                        CONVERSION(&polynome_coefficient[0]);
                        CONVERSION(&Settings);
                        switch(fract_type)
                        {
                            case 1:
                                CALCUL_1(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,choix);
                                break;
                            case 2:
                                CALCUL_2(ecran,polynome_coefficient,&Settings,&repere,&complexe,&Window_Settings);
                                break;
                            case 3:
                                CALCUL_3(ecran,&Settings,&repere,&complexe,&Window_Settings,choix,n);
                                break;
                            case 4:
                                CALCUL_4(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,n);
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDLK_KP_ENTER:
                        SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
                        CONVERSION(&polynome_coefficient[0]);
                        CONVERSION(&Settings);
                        switch(fract_type)
                        {
                            case 1:
                                CALCUL_1(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,choix);
                                break;
                            case 2:
                                CALCUL_2(ecran,polynome_coefficient,&Settings,&repere,&complexe,&Window_Settings);
                                break;
                            case 3:
                                CALCUL_3(ecran,&Settings,&repere,&complexe,&Window_Settings,choix,n);
                                break;
                            case 4:
                                CALCUL_4(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,n);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        choix=7;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        if(event.motion.x>NewFract.fond.positionImage.x && event.motion.x<(NewFract.fond.positionImage.x+NewFract.fond.image->w))
                        {
                            if(event.motion.y>NewFract.fond.positionImage.y && event.motion.y<(NewFract.fond.positionImage.y+NewFract.fond.image->h))
                            {
                                NewFract.ecriture.positionImage.x+=3;
                                NewFract.ecriture.positionImage.y+=3;
                            }
                        }
                        if(event.motion.x>Calcul.fond.positionImage.x && event.motion.x<(Calcul.fond.positionImage.x+Calcul.fond.image->w))
                        {
                            if(event.motion.y>Calcul.fond.positionImage.y && event.motion.y<(Calcul.fond.positionImage.y+Calcul.fond.image->h))
                            {
                                Calcul.ecriture.positionImage.x+=3;
                                Calcul.ecriture.positionImage.y+=3;
                            }
                        }
                        break;
                    case SDL_BUTTON_WHEELUP:
                        ZOOM(&event,&Window_Settings,ecran,&coordonnees,&zoom,fract_type);
                        Window_Settings.zoom=1;
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        ZOOM(&event,&Window_Settings,ecran,&coordonnees,&zoom,fract_type);
                        Window_Settings.zoom=1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        if(event.button.x>NewFract.fond.positionImage.x && event.button.x<(NewFract.fond.positionImage.x+NewFract.fond.image->w))
                        {
                            if(event.button.y>NewFract.fond.positionImage.y && event.button.y<(NewFract.fond.positionImage.y+NewFract.fond.image->h))
                            {
                                NewFract.ecriture.positionImage.x-=3;
                                NewFract.ecriture.positionImage.y-=3;
                                fract_type=CHOIX_TYPE_FRACTALE(ecran);
                                if(fract_type==1)
                                {
                                    Window_Settings.xMax=1;
                                    Window_Settings.xMin=-1;
                                    Window_Settings.yMax=1;
                                    Window_Settings.yMin=-1;
                                    Window_Settings.deltaX=2;
                                    Window_Settings.deltaY=2;
                                }
                                if(fract_type==2)
                                {
                                    Window_Settings.xMax=2;
                                    Window_Settings.xMin=-2;
                                    Window_Settings.yMax=1.5;
                                    Window_Settings.yMin=-1.5;
                                    Window_Settings.deltaX=4;
                                    Window_Settings.deltaY=3;
                                }
                                if(fract_type==3)
                                {
                                    Window_Settings.xMax=0.6;
                                    Window_Settings.xMin=-2;
                                    Window_Settings.yMax=1;
                                    Window_Settings.yMin=-1;
                                    Window_Settings.deltaX=2.6;
                                    Window_Settings.deltaY=2;
                                }
                                if(fract_type==4)
                                {
                                    Window_Settings.xMax=8;
                                    Window_Settings.xMin=-8;
                                    Window_Settings.yMax=6;
                                    Window_Settings.yMin=-6;
                                    Window_Settings.deltaX=16;
                                    Window_Settings.deltaY=12;
                                }
                                SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
                                if(fract_type!=2)
                                {
                                    PARAMETRES(ecran,&repere,&Settings,&polynome_coefficient[0],fract_type);
                                    if(fract_type==3 || fract_type==4)
                                    {
                                        DEGRE_MANDEL(ecran,n,&repere,&degre,fract_type);
                                        n=(int)degre.y;
                                    }
                                }
                                else
                                {
                                    for(i=10 ; i>=0 ; i--)
                                    {
                                        POLYNOME_SETTINGS(&polynome_coefficient[i],ecran);
                                        CONVERSION(&polynome_coefficient[i]);
                                    }

                                    SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
                                    SDL_Flip(ecran);
                                    PARAMETRES(ecran,&repere,&Settings,&polynome_coefficient[0],fract_type);

                                    SDL_BlitSurface(repere.image,NULL,ecran,&repere.positionImage);
                                }
                            }
                        }
                        if(event.button.x>Calcul.fond.positionImage.x && event.button.x<(Calcul.fond.positionImage.x+Calcul.fond.image->w))
                        {
                            if(event.button.y>Calcul.fond.positionImage.y && event.button.y<(Calcul.fond.positionImage.y+Calcul.fond.image->h))
                            {
                                Calcul.ecriture.positionImage.x-=3;
                                Calcul.ecriture.positionImage.y-=3;

                                CONVERSION(&polynome_coefficient[0]);
                                CONVERSION(&Settings);


                                switch(fract_type)
                                {
                                    case 1:
                                        CALCUL_1(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,choix);
                                        break;
                                    case 2:
                                        CALCUL_2(ecran,polynome_coefficient,&Settings,&repere,&complexe,&Window_Settings);
                                        break;
                                    case 3:
                                        CALCUL_3(ecran,&Settings,&repere,&complexe,&Window_Settings,choix,n);
                                        break;
                                    case 4:
                                        CALCUL_4(ecran,&Settings,&polynome_coefficient[0],&repere,&complexe,&Window_Settings,n);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        break;
                    case SDL_BUTTON_RIGHT:
                        break;
                    default:
                        break;
                }
            case SDL_MOUSEMOTION:
                if(event.motion.x>NewFract.fond.positionImage.x && event.motion.x<(NewFract.fond.positionImage.x+NewFract.fond.image->w))
                {
                    if(event.motion.y>NewFract.fond.positionImage.y && event.motion.y<(NewFract.fond.positionImage.y+NewFract.fond.image->h))
                    {
                        NewFract.fond.image=SDL_LoadBMP("Images/bouton2.BMP");
                    }
                    else
                    {
                        NewFract.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
                    }
                }
                else
                {
                    NewFract.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
                }

                if(event.motion.x>Calcul.fond.positionImage.x && event.motion.x<(Calcul.fond.positionImage.x+Calcul.fond.image->w))
                {
                    if(event.motion.y>Calcul.fond.positionImage.y && event.motion.y<(Calcul.fond.positionImage.y+Calcul.fond.image->h))
                    {
                        Calcul.fond.image=SDL_LoadBMP("Images/bouton2.BMP");
                    }
                    else
                    {
                        Calcul.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
                    }
                }
                else
                {
                    Calcul.fond.image=SDL_LoadBMP("Images/bouton1.BMP");
                }
                ADJUST_COORDONNEES(&event,&coordonnees,&repere,ecran,&Window_Settings,fract_type,&Window_Settings);
                break;
        }

        SDL_BlitSurface(NewFract.fond.image,NULL,ecran,&NewFract.fond.positionImage);
        SDL_BlitSurface(NewFract.ecriture.image,NULL,ecran,&NewFract.ecriture.positionImage);
        SDL_BlitSurface(Calcul.fond.image,NULL,ecran,&Calcul.fond.positionImage);
        SDL_BlitSurface(Calcul.ecriture.image,NULL,ecran,&Calcul.ecriture.positionImage);

        SDL_Flip(ecran);
    }

    TTF_CloseFont(policeMenu);

    SDL_FreeSurface(complexe.image);
    SDL_FreeSurface(repere.image);
    SDL_FreeSurface(barreMenu.image);
    SDL_FreeSurface(NewFract.fond.image);
    SDL_FreeSurface(NewFract.ecriture.image);
    SDL_FreeSurface(Calcul.fond.image);
    SDL_FreeSurface(Calcul.ecriture.image);
    SDL_FreeSurface(coordonnees.image);
    SDL_FreeSurface(zoom.fond.image);
    if(zoom.ecriture.image!=NULL)
    {
        SDL_FreeSurface(zoom.ecriture.image);
    }
    if(Settings.numeroDeLaLettre_reelle>0)
    {
        SDL_FreeSurface(Settings.Partie_Reelle.image);
    }
    if(Settings.numeroDeLaLettre_imaginaire>0)
    {
        SDL_FreeSurface(Settings.Partie_Imaginaire.image);
    }
    if(degre.numeroDeLaLettre_imaginaire>0)
    {
        SDL_FreeSurface(degre.Partie_Imaginaire.image);
    }

    for(i=0 ; i<11 ; i++)
    {
        if(polynome_coefficient[i].numeroDeLaLettre_imaginaire>0)
        {
            SDL_FreeSurface(polynome_coefficient[i].Partie_Imaginaire.image);
        }
        if(polynome_coefficient[i].numeroDeLaLettre_reelle>0)
        {
            SDL_FreeSurface(polynome_coefficient[i].Partie_Reelle.image);
        }
    }

    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

# endif

