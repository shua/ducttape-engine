
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER
#define DUCTTAPE_ENGINE_NETWORK_NETWORKMANAGER

#include <Network/NetworkManager.hpp>

#include <Core/Manager.hpp>
#include <Network/ConnectionsManager.hpp>
#include <Network/NetworkEvent.hpp>

#include <SFML/Network/UdpSocket.hpp>

#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

namespace dt {

/**
  * Manager for serializing events and sending them over network.
  * @see ConnectionsManager - Holds all connections of this manager.
  */
class DUCTTAPE_API NetworkManager : public Manager {
    Q_OBJECT
public:
    /**
      * Default constructor.
      */
    NetworkManager();

    /**
      * Destructor.
      */
    ~NetworkManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static NetworkManager* Get();

    /**
      * Binds the Socket used for the complete networking to the port given.
      * @param port The port to bind the socket to, or 0 to automatically select a free port.
      * @returns True if the binding was successful. Otherwise false.
      */
    bool BindSocket(uint16_t port = 0);

    /**
      * Connects to a remote device. This method sends a HandshakeEvent to that target.
      * @param target The remote device to connect to.
      */
    void Connect(Connection target);

    /**
      * Disconnects from a remote device. This method sends a GoodbyeEvent to that target.
      * @param target The remote device to disconnect from.
      */
    void Disconnect(Connection target);

    /**
      * Disconnects from all remote devices.
      * @see NetworkManager::Disconnect(Connection target)
      */
    void DisconnectAll();

    /**
      * Sends all pending Events from the queue.
      * @see NetworkManager::QueueEvent(NetworkEvent* event);
      */
    void SendQueuedEvents();

    /**
      * Queues an Event to be sent later.
      * @see NetworkManager::SendQueuedEvents();
      * @param event The NetworkEvent to be sent.
      */
    void QueueEvent(std::shared_ptr<NetworkEvent> event);

    /**
      * Receives and handles all events pending at the socket.
      */
    void HandleIncomingEvents();

    void HandleEvent(std::shared_ptr<NetworkEvent> e);

    /**
      * Registers a NetworkEvent as prototype for incoming packets.
      * @see Factory Pattern
      * @see NetworkEvent::Clone();
      * @param event A new instance of a NetworkEvent to be used for factory.
      */
    void RegisterNetworkEventPrototype(std::shared_ptr<NetworkEvent> event);

    /**
      * Creates a new instance of the prototype with the type ID given.
      * @see Factory Pattern
      * @see NetworkEvent::Clone();
      * @see Event::GetTypeID();
      * @param type_id The ID of the type of NetworkEvent to create an instance of.
      * @returns A new instance of the prototype with the type ID given.
      */
    std::shared_ptr<NetworkEvent> CreatePrototypeInstance(uint16_t type);

    /**
      * Returns a pointer to the ConnectionsManager.
      * @returns A pointer to the ConnectionsManager.
      */
    ConnectionsManager* GetConnectionsManager();

    /**
      * Registers a new string with a generated Id.
      * @param name The string to register.
      * @returns The new Id.
      */
    uint16_t RegisterEvent(const QString& name);

    /**
      * Attempts to register a string with a specific id
      * @param name The string to register.
      * @param id The id to register it to.
      * @returns true if neither string nor id have already been registered.
      */
    bool RegisterEvent(const QString& name, uint16_t id);

    /**
      * Unregisters string with id
      * @param name The string to unregister.
      * @returns true If successful.
      */
    bool UnregisterEvent(const QString& name);

    /**
      * Checks whether a string is already registered.
      * @param name The string to be checked.
      * @returns true If the string is already registered, otherwise false.
      */
    bool EventRegistered(const QString& name);

    /**
      * Checks whether an Id is already used.
      * @param id The Id to be checked.
      * @returns true If the Id is already used, otherwise false.
      */
    bool EventRegistered(uint16_t id);

    /**
      * Returns the Id for a string.
      * @param string The string to find.
      * @returns The Id for the string.
      */
    uint16_t GetEventId(const QString& string);

    /**
      * Returns the string for an Id.
      * @param id The Id to find.
      * @returns The string for the Id.
      */
    const QString& GetEventString(uint16_t id);

signals:
    void NewEvent(std::shared_ptr<dt::NetworkEvent> event);

private:
    /**
      * Sends an Event to its recipients.
      * @param event The event to send.
      */
    void _SendEvent(std::shared_ptr<NetworkEvent> event);

    ConnectionsManager mConnectionsManager;                     //!< The ConnectionsManager that manages all remote devices.
    std::deque<std::shared_ptr<NetworkEvent>> mQueue;           //!< The queue of Events to be send. @see NetworkManager::QueueEvent(NetworkEvent* event);
    std::vector<std::shared_ptr<NetworkEvent>> mNetworkEventPrototypes;    //!< The list of prototypes known to mankind :P
    sf::UdpSocket mSocket;                                      //!< The socket used for data transmissions over network.

    uint16_t mLastEventId;                                      //!< The Id used to register the last string with.
    std::map<uint16_t, QString> mEventIds;                      //!< The relation map between Ids/strings.
};

}

#endif
