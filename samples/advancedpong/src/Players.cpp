
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


#include "Players.hpp"
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

Player::Player() {}

Player::Player(float field_width, float field_height, float paddle_size, QString name)
    : Node(name),
      mSpeed(8.f),
      mFieldHeight(field_height),
      mFieldWidth(field_width),
      mPaddleSize(paddle_size),
      mBallPtr(nullptr) {}

void Player::SetBallPtr(dt::Node *ball) {
    mBallPtr = ball;
}

float Player::GetPaddleSize() {
    return mPaddleSize;
}

float Player::checkCollision(Ogre::Vector3 ball_pos) {
    if (ball_pos.z <= GetPosition().z + mPaddleSize / 2 && ball_pos.z >= GetPosition().z - mPaddleSize / 2) {
        //      get relative position of ball |  normalize it |   * by max deg | and add 1 so we don't accidently return 0
        float newBallDir = ((ball_pos.z - GetPosition().z) / (mPaddleSize / 2) * 70) + 1;
        if(newBallDir==0){
            // can't have it returning false, though the chances of you hitting exactly in the middle of the paddle are very slim.
            newBallDir=0.1;
        }
        return newBallDir;
    } else {
        return false;
    }
}

void Player::updateFrame(double simulation_frame_time) {
    if (mBallPtr != nullptr) {
        float ball_x = mBallPtr->GetPosition().x;
        if ((ball_x < 0 && ball_x >= GetPosition().x) || (ball_x > 0 && ball_x <= GetPosition().x)) {
            float move = (mBallPtr->GetPosition().z >= GetPosition().z ? mSpeed : - mSpeed) * simulation_frame_time;
            float new_z = GetPosition().z + move;
            if(new_z > (mFieldHeight / 2 - mPaddleSize / 2)) {
                new_z = mFieldHeight / 2 - mPaddleSize / 2;
            } else if (new_z < (- mFieldHeight / 2 + mPaddleSize / 2)) {
                new_z = - mFieldHeight / 2 + mPaddleSize / 2;
            }
            SetPosition(GetPosition().x, GetPosition().y, new_z);
        }
    }
}

void Player::shoot() {
    dt::Node* new_bullet = AddChildNode(new dt::Node);
    new_bullet->SetPosition(0, 0, 0);
    dt::MeshComponent* bullet_mesh = new_bullet->AddComponent(new dt::MeshComponent("Ball", "ball"));
    dt::PhysicsBodyComponent* bullet_body = new_bullet->AddComponent(new dt::PhysicsBodyComponent(bullet_mesh->GetName()));
    bullet_body->SetRestrictMovement(btVector3(1, 0, 1));
    dt::Logger::Get().Debug("Dir: " % dt::Utils::ToString((this->GetPosition().x < 0) * 2 - 1));
    bullet_body->SetCentralForce(btVector3(200 * ((GetPosition().x < 0) * 2 - 1), 0, 0));
}

RealPlayer::RealPlayer() {}

RealPlayer::RealPlayer(float field_width, float field_height, float paddle_size, QString name)
    : Player(field_width, field_height, paddle_size, name),
      mUpKey(OIS::KC_UP),
      mDownKey(OIS::KC_DOWN) {
}

void RealPlayer::updateFrame(double simulation_frame_time) {
    float move = 0;
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(mUpKey)) {
        move += 1;
    }
    if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(mDownKey)) {
        move -= 1;
    }
    float new_z = GetPosition().z + move * simulation_frame_time * mSpeed;
    if(new_z > (mFieldHeight / 2 - mPaddleSize / 2)) {
        new_z = mFieldHeight / 2 - mPaddleSize / 2;
    } else if (new_z < (- mFieldHeight / 2 + mPaddleSize / 2)) {
        new_z = - mFieldHeight / 2 + mPaddleSize / 2;
    }
    SetPosition(GetPosition().x, GetPosition().y, new_z);
}

RealPlayer* RealPlayer::BindDown(OIS::KeyCode down) {
    mDownKey = down;
    return this;
}

RealPlayer* RealPlayer::BindUp(OIS::KeyCode up) {
    mUpKey = up;
    return this;
}
