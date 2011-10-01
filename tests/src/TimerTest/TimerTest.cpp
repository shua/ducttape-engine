
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "TimerTest/TimerTest.hpp"

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/State.hpp>
#include <Utils/Timer.hpp>
#include <Utils/Utils.hpp>
#include <Scene/StateManager.hpp>

#include <QObject>

namespace TimerTest {

bool TimerTest::Run(int argc, char** argv) {
    dt::Game game;
    Main* main = new Main();
    game.Run(main, argc, argv);

    // we expect to have at least 9 ticks of timer 1 and 4 ticks of timer 2 within 10 seconds
    if(main->mTimer1Count < 9) {
        std::cerr << "Timer1 (thread, callback) did not tick often enough (only " << main->mTimer1Count << " times)." << std::endl;
        return false;
    }

    if(main->mTimer2Count < 4) {
        std::cerr << "Timer2 (thread) did not tick often enough (only " << main->mTimer2Count << " times)." << std::endl;
        return false;
    }

    return true;
}

QString TimerTest::GetTestName() {
    return "Timer";
}

////////////////////////////////////////////////////////////////

void Main::OnInitialize() {
    mTimer1Count = 0;
    mTimer2Count = 0;

    std::cout << "TIMER: Starting 2 timers:" << std::endl;
    std::cout << "  1 - Event mode  - 100ms" << std::endl;
    std::cout << "  2 - Thread mode - 200ms" << std::endl;
    std::cout << "  3 - Thread mode - 100ms - Callback, no events" << std::endl;

    mTimer1 = std::shared_ptr<dt::Timer>(new dt::Timer("Timer 1 (event mode)", 0.1, true, false));
    mTimer2 = std::shared_ptr<dt::Timer>(new dt::Timer("Timer 2 (thread mode)", 0.2, true, true));
    
    QObject::connect(mTimer1.get(), SIGNAL(TimerTicked(const QString&, double)), 
                     this, SLOT(_TimerCallback(QString)));
    QObject::connect(mTimer2.get(), SIGNAL(TimerTicked(const QString&, double)), 
                     this, SLOT(_TimerCallback(QString)), Qt::DirectConnection);
    
    QObject::connect(this, SIGNAL(BeginFrame(double)), this, SLOT(_HandleEvent(double)));
    mTotalTime = 0;
}

void Main::_HandleEvent(double simulation_frame_time) {
    mTotalTime += simulation_frame_time;

    if(mTotalTime >= 1.0) {
        dt::StateManager::Get()->Pop(1);
        mTimer1->Stop();
        mTimer2->Stop();
    }
}

void Main::_TimerCallback(const QString& message) {
    if(message == "Timer 1 (event mode)") {
        mTimer1Count++;
        std::cout << "Timer tick " << mTimer1Count << ": " << "Timer 1 (event mode)" << std::endl;
    }
    if(message == "Timer 2 (thread mode)") {
        mTimer2Count++;
        std::cout << "Timer tick " << mTimer2Count << ": " << "Timer 2 (thread mode)" << std::endl;
    }
}

} // namespace TimerTest
