
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <Scene/Node.hpp>
#include <Input/InputManager.hpp>
#include <Network/NetworkEvent.hpp>

class Player: public dt::Node {
    Q_OBJECT
public slots:
    virtual void updateFrame(double simulation_frame_time);

public:
    Player();
    Player(float field_width, float field_height, float paddle_size, QString name);

    bool checkCollision(Ogre::Vector3 ball_pos);
    float GetPaddleSize();

    void SetBallPtr(dt::Node* ball);

    float mSpeed;
    float mFieldHeight;
    float mFieldWidth;
    float mPaddleSize;

private:
    dt::Node* mBallPtr;
};

class RealPlayer: public Player {
    Q_OBJECT
public slots:
    void updateFrame(double simulation_frame_time);

public:
    RealPlayer();
    RealPlayer(float field_width, float field_height, float paddle_size, QString name);
    RealPlayer* BindUp(OIS::KeyCode up);
    RealPlayer* BindDown(OIS::KeyCode down);

private:
    OIS::KeyCode mUpKey;
    OIS::KeyCode mDownKey;
};

/*
class NetworkPlayer: public Player {
    Q_OBJECT
public slots:
    void updateFrame(bool simulation_frame_time);
    void HandleEvent(dt::NetworkEvent event);
};
*/

#endif // PLAYERS_HPP
