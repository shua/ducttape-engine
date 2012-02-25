#ifndef APONGCOMPONENTS_HPP
#define APONGCOMPONENTS_HPP

#include <Scene/Component.hpp>
#include <Input/InputManager.hpp>

class PlayerControl: public dt::Component {
    Q_OBJECT
public:
    void OnCreate();

    void SetUpKey(OIS::KeyCode k);
    void SetDnKey(OIS::KeyCode k);

public slots:
    void _keyEvent(OIS::KeyEvent event);

private:
    OIS::KeyCode _Up;
    OIS::KeyCode _Dn;
};

#endif // APONGCOMPONENTS_HPP
