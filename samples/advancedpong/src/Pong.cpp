#include "Pong.hpp"
#include <Core/Root.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Utils.hpp>
#include <Input/InputManager.hpp>
#include <Core/ResourceManager.hpp>
#include <OgreFontManager.h>

#include <OgreProcedural.h>

#include <OgreParticleAffector.h>

float FIELD_WIDTH = 24;
float FIELD_HEIGHT = 16;
float PADDLE_SIZE = 3.0;
QString music_file = "test_music_loop.ogg";

void Main::ResetBall() {
    if(mScore1 < 10 && mScore2 < 10) {
        mBallSpeed = Ogre::Vector3(4, -3, 0);
    } else {
        mBallSpeed = Ogre::Vector3::ZERO;
        music_component->PlayMusic();
        mesh->PlayAnimation();
        QString p(mScore1 == 10 ? "left" : "right");
<<<<<<< HEAD
        GetScene("testscene")->FindChildNode("info")->FindComponent<dt::TextComponent>("text")->SetText("The " % p % " player wins the game.");
	    mOgreNode->SetPosition(Ogre::Vector3(0, 0, 10));
=======
        GetScene("testscene")->FindChildNode("info")->FindComponent<dt::TextComponent>("text")->SetText("The " + p + " player wins the game.");
>>>>>>> 79a5040da23f7b10480246be5273cd0b037922b2
    }
    mBallNode->SetPosition(Ogre::Vector3(0,0,0));
    mScore1Text->SetText(dt::Utils::ToString(mScore1));
    mScore2Text->SetText(dt::Utils::ToString(mScore2));
}

void Main::OnInitialize() {
    mScore1 = 0;
    mScore2 = 0;

    dt::Scene* scene = AddScene(new dt::Scene("testscene"));
    scene->GetPhysicsWorld()->SetGravity(Ogre::Vector3(0, 0, 0));
    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
    dt::ResourceManager::Get()->AddResourceLocation("particle/","FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("advancedpong/","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");

    dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
    camnode->SetPosition(Ogre::Vector3(0, 0, 30));
    camnode->AddComponent(new dt::CameraComponent("cam"))->LookAt(Ogre::Vector3(0, 0, 0));

    // generate meshes
    OgreProcedural::BoxGenerator().setSizeX(FIELD_WIDTH + 1).setSizeY(FIELD_HEIGHT).setSizeZ(1.f).realizeMesh("Field");
    //OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(1.f).setSizeZ(1.f).realizeMesh("Ball");
    OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(3.f).setSizeZ(1.f).realizeMesh("Paddle");
	OgreProcedural::SphereGenerator().setRadius(0.65).setNumRings(10).setNumSegments(16).realizeMesh("Ball");

    mGameNode = scene->AddChildNode(new dt::Node("game"));

    dt::Node* music_node = scene->AddChildNode(new dt::Node("music"));
    music_component = music_node->AddComponent(new dt::MusicComponent(music_file));
	music_component->StopMusic();

    dt::Node* hitsound = scene->AddChildNode(new dt::Node("hitsound"));
    sound_component = new dt::SoundComponent("ballhit.wav");
    hitsound->AddComponent(sound_component);
    sound_component->SetMasterVolume(100.0f);
    

    dt::Node* FieldNode = scene->AddChildNode(new dt::Node("fieldnode"));
    FieldNode->SetPosition(Ogre::Vector3(0, 0, 0));
    FieldNode->AddComponent(new dt::BillboardSetComponent("billboard_test", 1, "fieldbackground.png"));
    dt::BillboardSetComponent* billboardSetComponent = FieldNode->FindComponent<dt::BillboardSetComponent>("billboard_test");
    Ogre::BillboardSet* billboardSet = billboardSetComponent->GetOgreBillboardSet();
    billboardSet->setDefaultDimensions(FIELD_WIDTH, FIELD_HEIGHT);

    mBallNode = mGameNode->AddChildNode(new dt::Node("ball"));
    mBallNode->SetPosition(Ogre::Vector3(0, 0, 0));
    mBallNode->AddComponent(new dt::MeshComponent("Ball", "SimplePongBall", "mesh"));
    mBallNode->AddComponent(new dt::PhysicsBodyComponent("mesh", "ball-body"));

    dt::Node* lightnode = mGameNode->AddChildNode(new dt::Node("lightnode"));
    lightnode->SetPosition(Ogre::Vector3(0, 0, 15));
    lightnode->AddComponent(new dt::LightComponent("light"));

    mPaddle1Node = mGameNode->AddChildNode(new dt::Node("paddle1"));
    mPaddle1Node->SetPosition(Ogre::Vector3(- FIELD_WIDTH / 2 - 0.5, 0, 0));
    mPaddle1Node->AddComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "paddle1mesh"));
    mPaddle1Node->AddComponent(new dt::PhysicsBodyComponent("paddle1mesh", "paddle1-body"));

    mPaddle2Node = mGameNode->AddChildNode(new dt::Node("paddle2"));
    mPaddle2Node->SetPosition(Ogre::Vector3(FIELD_WIDTH / 2 + 0.5, 0, 0));
    mPaddle2Node->AddComponent(new dt::MeshComponent("Paddle", "SimplePongPaddle", "paddle2mesh"));
    mPaddle2Node->AddComponent(new dt::PhysicsBodyComponent("paddle2mesh", "paddle2-body"));

    dt::Node* score1_node = mGameNode->AddChildNode(new dt::Node("score1"));
    score1_node->SetPosition(Ogre::Vector3(-10, FIELD_HEIGHT / 2 + 2, 1));
    mScore1Text = score1_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore1Text->SetFont("DejaVuSans");
    mScore1Text->SetFontSize(64);

    dt::Node* score2_node = mGameNode->AddChildNode(new dt::Node("score2"));
    score2_node->SetPosition(Ogre::Vector3(10, FIELD_HEIGHT / 2 + 2, 1));
    mScore2Text = score2_node->AddComponent(new dt::TextComponent("0", "text"));
    mScore2Text->SetFont("DejaVuSans");
    mScore2Text->SetFontSize(64);

    dt::Node* info_node = scene->AddChildNode(new dt::Node("info"));
    info_node->SetPosition(Ogre::Vector3(0, - FIELD_HEIGHT / 2 - 3, 1));
    dt::TextComponent* info_text = info_node->AddComponent(new dt::TextComponent("Left player: W/S -- Right player: Up/Down", "text"));
    info_text->SetFont("DejaVuSans");
    info_text->SetFontSize(20);

    mOgreNode = mGameNode->AddChildNode(new dt::Node("meshnode"));
    mesh = new dt::MeshComponent("Sinbad.mesh");
    mOgreNode->AddComponent(mesh);
    mesh->SetAnimation("Dance");
    mesh->SetLoopAnimation(true);
    mesh->SetCastShadows(false);
    //mesh->PlayAnimation(); will only play animation when the game is won by either player
    mOgreNode->SetPosition(Ogre::Vector3(0, 0, -10));

    // create the particle system for the ball
    dt::ParticleSystemComponent* particleSys = mBallNode->AddComponent(new dt::ParticleSystemComponent("particleSys"));
    particleSys->SetMaterialName("Test/Particle");
    particleSys->SetParticleCountLimit(500);
    particleSys->GetOgreParticleSystem()->setDefaultDimensions(1.7, 1.7);

    Ogre::ParticleEmitter* emitter = particleSys->AddEmitter("emit1", "Point");
    emitter->setAngle(Ogre::Degree(20));
    emitter->setColour(Ogre::ColourValue(1.f, 0.6f, 0.f), Ogre::ColourValue(0.2f, 0.8f, 0.2f));
    emitter->setEmissionRate(100);
    emitter->setParticleVelocity(-1.5f, 1.5f);
    emitter->setTimeToLive(0.2f, 0.75f);

    particleSys->AddScalerAffector("scaler", -1.0);
    particleSys->AddLinearForceAffector("force", Ogre::Vector3(0, 0, 0));

    Ogre::ParticleAffector* affector = particleSys->AddAffector("colour_interpolator", "ColourInterpolator");
    affector->setParameter("time0", "0");
    affector->setParameter("colour0", "1 1 0 1");
    affector->setParameter("time1", "0.5");
    affector->setParameter("colour1", "1 0.5 0 1");
    affector->setParameter("time2", "1");
    affector->setParameter("colour2", "1 0 0 0");

    ResetBall();
}

void Main::UpdateStateFrame(double simulation_frame_time) {

    /*
    mBallSpeed *= 1.0 + (simulation_frame_time * 0.05);

    // move paddle 1
    float move1 = 0;
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W)) {
        move1 += 2;
    }
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S)) {
        move1 -= 2;
    }
    float new_y1 = mPaddle1Node->GetPosition().y + move1 * simulation_frame_time * 8;
    if(new_y1 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)
        new_y1 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    else if(new_y1 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)
        new_y1 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

    mPaddle1Node->SetPosition(Ogre::Vector3(
                mPaddle1Node->GetPosition().x,
                new_y1,
                mPaddle1Node->GetPosition().z));

    // move paddle 2
    float move2 = 0;
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_UP)) {
        move2 += 2;
    }

    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_DOWN)) {
        move2 -= 2;
    }

    float new_y2 = mPaddle2Node->GetPosition().y + move2 * simulation_frame_time * 8;
    if(new_y2 > FIELD_HEIGHT / 2 - PADDLE_SIZE / 2)new_y2 = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    else if(new_y2 < - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2)new_y2 = - FIELD_HEIGHT / 2  + PADDLE_SIZE / 2;

    mPaddle2Node->SetPosition(Ogre::Vector3(mPaddle2Node->GetPosition().x,new_y2,mPaddle2Node->GetPosition().z));

    // move ball
    Ogre::Vector3 newpos(mBallNode->GetPosition() + mBallSpeed * simulation_frame_time);
    if(newpos.y >= FIELD_HEIGHT / 2 - 0.5 || newpos.y <= -FIELD_HEIGHT / 2 + 0.5) {
        mBallSpeed.y *= -1;
        //sound_component->PlaySound(); <- somtimes won't play if the sound was played just before, for example if it hits the wall, then the paddle immediately it won't play the sound when it hits the paddle.
    }

    if(newpos.x >= FIELD_WIDTH / 2 - 0.5) {
        float paddle_y = mPaddle2Node->GetPosition().y;
        if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
            dt::Logger::Get().Info(" Right Player lost!");
            ++mScore1;
            ResetBall();
        } else {
            mBallSpeed.x *= 1.1;
            mBallSpeed.x *= -1;
            sound_component->PlaySound();
        }
    } else if(newpos.x <= -FIELD_WIDTH / 2 + 0.5) {
        float paddle_y = mPaddle1Node->GetPosition().y;
        if(newpos.y < paddle_y - PADDLE_SIZE / 2 - 0.5 || newpos.y > paddle_y + PADDLE_SIZE / 2 + 0.5) {
            dt::Logger::Get().Info(" Left Player lost!");
            ++mScore2;
            ResetBall();
        } else {
            mBallSpeed.x *= 1.1;
            mBallSpeed.x *= -1;
            sound_component->PlaySound();
        }
    }

    mBallNode->SetPosition(mBallNode->GetPosition() + mBallSpeed * simulation_frame_time);

    
    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Radian(Ogre::Math::Cos(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_X);
    Ogre::Quaternion w;
    w.FromAngleAxis(Ogre::Radian(Ogre::Math::Sin(Ogre::Radian(dt::Root::GetInstance().GetTimeSinceInitialize() * 0.2))) * 0.1, Ogre::Vector3::UNIT_Y);
    mGameNode->SetRotation(q * w);
    */
}
