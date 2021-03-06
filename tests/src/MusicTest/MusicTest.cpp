
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "MusicTest/MusicTest.hpp"

#include <Core/ResourceManager.hpp>

#include <SFML/System.hpp>

namespace MusicTest {

bool MusicTest::Run(int argc, char** argv) {
    dt::Root& root = dt::Root::GetInstance();
    root.Initialize(argc, argv);
    QString music_file = "test_music_loop.ogg";

    // set global volume
    //dt::MusicComponent::SetMasterVolume(2);

    dt::Scene scene("scene1");

    dt::Node* music_node = scene.AddChildNode(new dt::Node("music"));
    dt::MusicComponent* music_component = music_node->AddComponent(new dt::MusicComponent(music_file));

    music_component->SetVolume(40);

    auto resmgr = root.GetResourceManager();

    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(sf::Milliseconds(500));
    //dt::MusicComponent::SetMasterVolume(1);
    music_component->SetVolume(1);
    sf::Sleep(sf::Milliseconds(500));

    if(resmgr->GetMusicFile(music_file)->GetPlayingOffset().AsMilliseconds() < 100) {
        std::cerr << "The music was not played correctly." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    music_component->PauseMusic();
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Paused) {
        std::cerr << "The music is currently playing. It should be paused." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(sf::Milliseconds(200));

    music_component->StopMusic();
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Stopped) {
        std::cerr << "The music is currently not stopped." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    sf::Sleep(sf::Milliseconds(200));

    music_component->PlayMusic();
    if(resmgr->GetMusicFile(music_file)->GetStatus() != sf::Music::Playing) {
        std::cerr << "The music is currently not playing." << std::endl;
        resmgr->GetMusicFile(music_file)->Stop();
        return false;
    }

    resmgr->GetMusicFile(music_file)->Stop();
    root.Deinitialize();
    return true;
}

QString MusicTest::GetTestName() {
    return "Music";
}

}
