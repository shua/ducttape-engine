
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef ADVANCEDPONG_HPP
#define ADVANCEDPONG_HPP

#include "Players.hpp"

#include <Core/Root.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/TextComponent.hpp>
#include <Audio/MusicComponent.hpp>
#include <Audio/SoundComponent.hpp>
#include <Scene/StateManager.hpp>
#include <Scene/Node.hpp>
#include <Gui/GuiManager.hpp>

#include <OgreVector3.h>

class MainScreen: public dt::State {
    Q_OBJECT
public:
    MainScreen();
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

    void Click(MyGUI::Widget* _sender);
private:
    MyGUI::EditBox* mPlayers;
};

class AStrangeGame: public dt::State {
    Q_OBJECT
public:
    AStrangeGame();
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

    void Click(MyGUI::Widget* _sender);
};

class AdvancedPong: public dt::State {
    Q_OBJECT
public:
    AdvancedPong(QString players);
    void ResetBall();

    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

    void InitializePlayers();

    void GuiClick(MyGUI::Widget* _sender);

public slots:
    // Have to use this so it won't switch between pause and unpause with key repeats.
    void KeyPressed(const OIS::KeyEvent& event);

signals:
    void UpdateFrameSignal(double simulation_frame_time);

private:
    dt::Node* mGameNode;
    dt::Node* mFieldNode;
    dt::Node* mBallNode;
    Player* mPlayer1;
    Player* mPlayer2;
    dt::TextComponent* mScore1Text;
    dt::TextComponent* mScore2Text;
    dt::TextComponent* mInfoText;
    dt::SoundComponent* mSoundComponent;
    dt::MusicComponent* mMusicComponent;
    dt::MeshComponent* mMesh;

    bool mPaused;
    bool keyPressed;
    int mPlayers;
    int mScore1;
    int mScore2;

    Ogre::Vector3 mBallSpeed;
};

#endif // ADVANCEDPONG_HPP
