
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "AdvancedPong.hpp"

#include <Core/ResourceManager.hpp>
#include <Utils/Utils.hpp>
#include <Scene/Scene.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Gui/GuiButton.hpp>
#include <Input/InputManager.hpp>

#include <OgreFontManager.h>
#include <OgreParticleAffector.h>
#include <OgreProcedural.h>

MainScreen::MainScreen() {}

void MainScreen::Click(MyGUI::Widget* _sender) {
    QString name = dt::Utils::ToString(_sender->getName());
    if(name == "Gui.play_single") {
        dt::StateManager::Get()->SetNewState(new AdvancedPong(1));
    } else if(name == "Gui.play_double") {
        dt::StateManager::Get()->SetNewState(new AdvancedPong(2));
    } else if(name == "Gui.quit") {
        dt::StateManager::Get()->Pop(1);
    }
}

void MainScreen::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("mainscreen"));

    dt::ResourceManager::Get()->AddResourceLocation("gui", "FileSystem", true);
    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(0, 0, 0);
    camnode->AddComponent(new dt::CameraComponent("camera"))->LookAt(Ogre::Vector3(0, 1, 0));

    dt::GuiRootWindow& win = dt::GuiManager::Get()->GetRootWindow();

    dt::GuiButton* playsingle = win.CreateChild<dt::GuiButton>("play_single");
    playsingle->SetCaption("Play Computer");
    playsingle->SetPosition(400, 200);
    playsingle->SetSize(200, 30);
    dynamic_cast<MyGUI::Button*>(playsingle->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &MainScreen::Click);

    dt::GuiButton* playdouble = win.CreateChild<dt::GuiButton>("play_double");
    playdouble->SetCaption("Play VS");
    playdouble->SetPosition(400, 240);
    playdouble->SetSize(200, 30);
    dynamic_cast<MyGUI::Button*>(playdouble->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &MainScreen::Click);

    dt::GuiButton* quit = win.CreateChild<dt::GuiButton>("quit");
    quit->SetCaption("Quit");
    quit->SetPosition(400, 280);
    quit->SetSize(200, 30);
    dynamic_cast<MyGUI::Button*>(quit->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &MainScreen::Click);
}

void MainScreen::UpdateStateFrame(double simulation_frame_time) {}

// -----------AdvancedPong State---------

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;
QString music_file = "test_music_loop.ogg";

AdvancedPong::AdvancedPong(int players)
    : mPaused(0),
      mPlayers(players),
      mScore1(0),
      mScore2(0) {}

void AdvancedPong::InitializePlayers() {
    if(mPlayers == 0) {
        //dt::StateManager::Get()->SetNewState(new AStrangeGame());
    } else if(mPlayers == 1) {
        mPlayer1 = (new Player(FIELD_WIDTH,FIELD_HEIGHT, PADDLE_SIZE, "Pongomatic-2000"));
        connect(this, SIGNAL(UpdateFrameSignal(double)), mPlayer1, SLOT(updateFrame(double)));
        mPlayer1->SetBallPtr(mBallNode);
        dt::Node* mPaddle1Node = mGameNode->AddChildNode(mPlayer1);
        mPaddle1Node->SetPosition((- FIELD_WIDTH / 2 - 0.5), 0, 0);
        mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "paddle", "paddle1mesh"));

        mPlayer2 = (new RealPlayer(FIELD_WIDTH, FIELD_HEIGHT, PADDLE_SIZE, "Bob"));
        connect(this, SIGNAL(UpdateFrameSignal(double)), mPlayer2, SLOT(updateFrame(double)));
        dt::Node* mPaddle2Node = mGameNode->AddChildNode(mPlayer2);
        mPaddle2Node->SetPosition((FIELD_WIDTH / 2 + 0.5), 0, 0);
        mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "paddle", "paddle2mesh"));
    } else if(mPlayers == 2) {
        RealPlayer* human(new RealPlayer(FIELD_WIDTH, FIELD_HEIGHT, PADDLE_SIZE, "Steve"));
        human->BindUp(OIS::KC_W)->BindDown(OIS::KC_S);
        mPlayer1 = human;
        connect(this, SIGNAL(UpdateFrameSignal(double)), mPlayer1, SLOT(updateFrame(double)));
        dt::Node* mPaddle1Node = mGameNode->AddChildNode(mPlayer1);
        mPaddle1Node->SetPosition((- FIELD_WIDTH / 2 - 0.5), 0, 0);
        mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "paddle", "paddle1mesh"));

        mPlayer2 = (new RealPlayer(FIELD_WIDTH, FIELD_HEIGHT, PADDLE_SIZE, "Bob"));
        connect(this, SIGNAL(UpdateFrameSignal(double)), mPlayer2, SLOT(updateFrame(double)));
        dt::Node* mPaddle2Node = mGameNode->AddChildNode(mPlayer2);
        mPaddle2Node->SetPosition((FIELD_WIDTH / 2 + 0.5), 0, 0);
        mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "paddle", "paddle2mesh"));
    }
}

void AdvancedPong::ResetBall() {
    if (mScore1 < 10 && mScore2 < 10) {
        mBallSpeed = Ogre::Vector3(4, 0, 3);
    } else {
        mBallSpeed = Ogre::Vector3::ZERO;
        mMusicComponent->PlayMusic();
        //mMesh->PlayAnimation();
        //QString winner(mScore1 == 10 ? "left" : "right");
        //mOgreNode->SetPosition(Ogre::Vector3(0, 10, 0));
    }
    mBallNode->SetPosition(Ogre::Vector3(0, 0, 0));
    mScore1Text->SetText(dt::Utils::ToString(mScore1));
    mScore2Text->SetText(dt::Utils::ToString(mScore2));
}

void AdvancedPong::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("AdvancedPong"));
    scene->GetPhysicsWorld()->SetGravity(Ogre::Vector3(0, 0, 0));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("advancedpong", "FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("particle", "FileSystem", true);
    Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
    Ogre::FontManager::getSingletonPtr()->load("DejaVuSans", "General");

    dt::Node* cam_node = scene->AddChildNode(new dt::Node("camnode"));
    cam_node->SetPosition(0, -30, 0);
    cam_node->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(1.f).setSizeZ(FIELD_HEIGHT).realizeMesh("Field");
    OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(1.f).setSizeZ(3.f).realizeMesh("Paddle");
    OgreProcedural::SphereGenerator().setRadius(.65).setNumRings(10).setNumSegments(16).realizeMesh("Ball");

    // add some pre-game stuff
    dt::Node* music_node = scene->AddChildNode(new dt::Node("music"));
    music_node->AddComponent(new dt::MusicComponent(music_file))->StopMusic();

    dt::Node* hit_sound = scene->AddChildNode(new dt::Node("hitsound"));
    hit_sound->AddComponent(new dt::SoundComponent("ballhit.wav"))->SetMasterVolume(100.0f);

    dt::Node* light_node = scene->AddChildNode(new dt::Node("lightnode"));
    light_node->SetPosition(0, -15, 0);
    light_node->AddComponent(new dt::LightComponent("light"))->SetColor(Ogre::ColourValue(0.5, 0.5, 0.5));

    // game mechanics
    mGameNode = scene->AddChildNode(new dt::Node("gamenode"));

    dt::Node* field_node = scene->AddChildNode(new dt::Node("feildnode"));
    field_node->SetPosition(0, 1, 0);
    field_node->AddComponent(new dt::MeshComponent("Field", "field", "fieldmesh"));

    mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
    mBallNode->SetPosition(0, -1, 0);
    mBallNode->AddComponent(new dt::MeshComponent("Ball", "ball", "ballmesh"));
    dt::ParticleSystemComponent* ball_p = mBallNode->AddComponent(new dt::ParticleSystemComponent("ball_p"));
    ball_p->SetMaterialName("Test/Particle");
    ball_p->SetParticleCountLimit(500);
    ball_p->GetOgreParticleSystem()->setDefaultDimensions(1.7, 1.7);
    Ogre::ParticleEmitter* ball_pe = ball_p->AddEmitter("ball_pe", "Point");
    ball_pe->setAngle(Ogre::Degree(20));
    ball_pe->setColour(Ogre::ColourValue(1.f, 0.6, 0.f), Ogre::ColourValue(0.2, 0.8, 0.2));
    ball_pe->setEmissionRate(100);
    ball_pe->setParticleVelocity(-1.5, 1.5);
    ball_pe->setTimeToLive(0.2, 0.75);
    ball_p->AddScalerAffector("scaler", -1.0);
    ball_p->AddLinearForceAffector("force", Ogre::Vector3(0, 0, 0));
    Ogre::ParticleAffector* ball_paffector = ball_p->AddAffector("colour_interpolater", "ColourInterpolator");
    ball_paffector->setParameter("time0", "0");
    ball_paffector->setParameter("colour0", "1 1 0 1");
    ball_paffector->setParameter("time1", "0.5");
    ball_paffector->setParameter("colour1", "1 0.5 0 1");
    ball_paffector->setParameter("time2", "1");
    ball_paffector->setParameter("colour2", "1 0 0 0");

    InitializePlayers();

    dt::Node* score1_node = mGameNode->AddChildNode(new dt::Node("score1"));
    score1_node->SetPosition(10, 1, (FIELD_HEIGHT / 2 + 2));
    mScore1Text = score1_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore1Text->SetFont("DejaVuSans");
    mScore1Text->SetFontSize(64);

    dt::Node* score2_node = mGameNode->AddChildNode(new dt::Node("score2"));
    score2_node->SetPosition(-10, 1, (FIELD_HEIGHT / 2 + 2));
    mScore2Text = score2_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore2Text->SetFont("DejaVuSans");
    mScore2Text->SetFontSize(64);

    dt::Node* info_node = scene->AddChildNode(new dt::Node("info"));
    info_node->SetPosition(0, 0, (- FIELD_HEIGHT / 2 - 3));
    if(mPlayers == 1) {
        mInfoText = info_node->AddComponent(new dt::TextComponent("Controls: Up/Down"));
    } else if(mPlayers == 2) {
        mInfoText = info_node->AddComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    }
    mInfoText->SetFont("DejaVuSans");
    mInfoText->SetFontSize(20);

    dt::GuiRootWindow& win = dt::GuiManager::Get()->GetRootWindow();
    dt::GuiButton* exit = win.CreateChild<dt::GuiButton>("exit");
    exit->SetCaption("Exit to Main Screen");
    exit->SetPosition(10, 10);
    exit->SetSize(200, 30);
    //exit->GetMyGUIWidget()->setAlpha(0);
    //exit->GetMyGUIWidget()->setEnabled(false);
    dynamic_cast<MyGUI::Button*>(exit->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &AdvancedPong::GuiClick);

    connect(dt::InputManager::Get(), SIGNAL(sKeyPressed(const OIS::KeyEvent&)), this, SLOT(KeyPressed(const OIS::KeyEvent&)));

    ResetBall();
}

void AdvancedPong::GuiClick(MyGUI::Widget* _sender) {
    if(_sender->getName() == "Gui.exit") {
        dt::StateManager::Get()->Pop();
    }
}

void AdvancedPong::KeyPressed(const OIS::KeyEvent& event) {
    if(event.key == OIS::KC_P || event.key == OIS::KC_ESCAPE) {
        if(mPaused) {
            mPaused = false;
        } else {
            mPaused = true;
        }
        if(event.key == OIS::KC_ESCAPE) {
            MyGUI::Gui* gui = dt::GuiManager::Get()->GetGuiSystem();
            MyGUI::Widget* exitbutton = gui->findWidgetT("Gui.exit");
            if(exitbutton->getAlpha() == 1) {
                exitbutton->setAlpha(0);
                exitbutton->setEnabled(false);
            } else {
                exitbutton->setAlpha(1);
                exitbutton->setEnabled(true);
            }
        }
    }
}

void AdvancedPong::UpdateStateFrame(double simulation_frame_time) {
    if(!mPaused) {
        emit UpdateFrameSignal(simulation_frame_time);

        // move ball
        mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

      Ogre::Vector3 newpos(mBallNode->GetPosition() + mBallSpeed * simulation_frame_time);
      if((newpos.z >= (FIELD_HEIGHT / 2 - 0.5)) || (newpos.z <= (- FIELD_HEIGHT / 2 + 0.5))) {
            mBallSpeed.z *= -1;
        }
        //mInfoText->SetText(dt::Utils::ToString(mPlayer1->GetPosition().z));
        if(newpos.x <= (- FIELD_WIDTH / 2 + 0.5)) {
            if(!mPlayer1->checkCollision(mBallNode->GetPosition())) {
                dt::Logger::Get().Info(mPlayer1->GetName() + " lost!");
                ++mScore1;
                ResetBall();
            } else {
                mBallSpeed.x *= -1;
            }
        } else if (newpos.x >= (FIELD_WIDTH / 2 - 0.5)) {
            if(!mPlayer2->checkCollision(mBallNode->GetPosition())) {
                dt::Logger::Get().Info(mPlayer2->GetName() + " lost!");
                ++mScore2;
                ResetBall();
            } else {
                mBallSpeed.x *= -1;
            }
        }

        mBallNode->SetPosition(mBallNode->GetPosition() + mBallSpeed * simulation_frame_time);
    }


    /*
    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Radian(Ogre::Math::Cos(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_X);
    Ogre::Quaternion w;
    w.FromAngleAxis(Ogre::Radian(Ogre::Math::Sin(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_Y);
    mGameNode->SetRotation(q * w);
    */
}
