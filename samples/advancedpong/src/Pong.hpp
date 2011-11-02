#include <Graphics/TextComponent.hpp>
#include <Graphics/ParticleSystemComponent.hpp>
#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Audio/MusicComponent.hpp>
#include <Audio/SoundComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/State.hpp>
#include <Physics/PhysicsWorld.hpp>
#include <OgreVector3.h>

class Main: public dt::State {
    Q_OBJECT
public:
    void ResetBall();

    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
    dt::Node* mGameNode;
    dt::Node* mFieldNode;
    dt::Node* mBallNode;
    dt::Node* mOgreNode;
    dt::Node* mPaddle1Node;
    dt::Node* mPaddle2Node;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    dt::SoundComponent* sound_component;
    dt::MusicComponent* music_component;
    dt::MeshComponent* mesh;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;
};
