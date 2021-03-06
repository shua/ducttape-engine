
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "BillboardTest/BillboardTest.hpp"

namespace BillboardTest {

bool BillboardTest::Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return true;
}

QString BillboardTest::GetTestName() {
    return "Billboard";
}

////////////////////////////////////////////////////////////////

Main::Main()
    : mRuntime(0) {}

void Main::UpdateStateFrame(double simulation_frame_time) {
    mRuntime += simulation_frame_time;
    mCamNode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 1, 0));
    if(mRuntime > 5.0) {
        dt::StateManager::Get()->Pop(1);
    }
}

void Main::OnInitialize() {
    dt::Scene* scene = AddScene(new dt::Scene("testscene"));

    dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem");
    dt::ResourceManager::Get()->AddResourceLocation("crate", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();

    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(.5f).realizeMesh("Sphere");
    OgreProcedural::PlaneGenerator().setSizeX(10.f).setSizeY(10.f).realizeMesh("Plane");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mCamNode = scene->AddChildNode(new dt::Node("camnode"));
    mCamNode->AddComponent(new dt::CameraComponent("cam"));
    mCamNode->SetPosition(Ogre::Vector3(10, 10, 10));
    mCamNode->FindComponent<dt::CameraComponent>("cam")->LookAt(Ogre::Vector3(0, 0, 0));
    dt::FollowPathComponent* path = mCamNode->AddComponent(new dt::FollowPathComponent(dt::FollowPathComponent::LOOP, "path"));
    path->SetFollowRotation(false);
    path->SetSmoothAcceleration(true);
    path->SetSmoothCorners(false);
    path->AddPoint(Ogre::Vector3(10, 10, 10));
    path->AddPoint(Ogre::Vector3(-10, 10, 10));
    path->AddPoint(Ogre::Vector3(10, -10, 10));
    path->SetDuration(5.0);


    dt::Node* cratenode = scene->AddChildNode(new dt::Node("spherenode"));
    cratenode->SetPosition(Ogre::Vector3(1, 1, 0));
    cratenode->AddComponent(new dt::MeshComponent("Crate01.mesh", "", "sphere-mesh"));

    dt::Node* billboardnode = scene->AddChildNode(new dt::Node("billboardnode"));
    billboardnode->SetPosition(Ogre::Vector3(0, 2, 0));
    billboardnode->AddComponent(new dt::BillboardSetComponent(
                                    "billboard_test", 1, "ducttape-logo-256x256.png"));
    dt::BillboardSetComponent* billboardSetComponent = billboardnode->
            FindComponent<dt::BillboardSetComponent>("billboard_test");
    Ogre::BillboardSet* billboardSet = billboardSetComponent
                                       ->GetOgreBillboardSet();
    billboardSet->setDefaultDimensions(2, 2);

    dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
    planenode->SetPosition(Ogre::Vector3(0, 0, 0));
    planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));

    dt::Node* lightnode1 = scene->AddChildNode(new dt::Node("lightnode1"));
    lightnode1->AddComponent(new dt::LightComponent("light1"));
    lightnode1->SetPosition(Ogre::Vector3(15, 5, 15));
}

} // namespace BillboardTest
