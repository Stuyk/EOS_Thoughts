#include "src.hpp"

namespace EOSThoughts {
    void Thoughts::create( account_name account, string message, string memo) {
        require_auth( account );

        // Ensure that the length isn't too long and the person creating the message is not the contract.
        eosio_assert( account != _self, "Cannot create thoughts from platform.");
        eosio_assert( message.length() <= 255, "Cannot be longer than 255 characters.");

        auto user = _users.find( account );
        bool newUser = false;

        // Create the user account if it does not exist.
        if (user == _users.end()) {
            _users.emplace( _self, [&](auto& x) {
                x.owner = account;
            });
            newUser = true;
        }

        // If the user is new we need to re-iterate.
        if (newUser)
            user = _users.find( account );

        // Modify the users table and add the new thought.
        _users.modify(user, account, [&](auto& x) {
            uint64_t key = 0;
            if (!newUser) {
                thought first = x.thoughts.front();
                key = first.key + 1; // Add 1 so it does not overwrite older keys.
            }

            // Insert this thought to the front of the array. We can use the array as a way to see the 'Latest' thoughts.
            x.thoughts.insert(x.thoughts.begin(), thought { key, message });
        });
    }


    void Thoughts::remove( account_name account, uint64_t key, string memo ) {
        require_auth( account );

        // Find the user.
        auto user = _users.find( account );
        eosio_assert(user != _users.end(), "User not found.");

        // Modify and remove the old message based on the key it is assigned.
        _users.modify(user, account, [&](auto& x) {
            // Get our message based on the key value.
            auto reference = find_if(x.thoughts.begin(), x.thoughts.end(), [=] (thought const& y) {
                return (y.key == key);
            });

            // Determine if the message was found.
            eosio_assert(reference != x.thoughts.end(), "Message was not found.");
            
            // Determine indexed position in vector.
            int index = distance(x.thoughts.begin(), reference);

            // Remove message.
            x.thoughts.erase(x.thoughts.begin() + index);
        });
    }

    void Thoughts::addfollower( account_name account, account_name who, string memo ) {
        require_auth( account );

        // Find the user.
        auto user = _users.find(account);
        eosio_assert(user != _users.end(), "User not found.");

        // Modify the user and add the new follower.
        _users.modify(user, account, [&](auto& x) {
            bool containsUser = false;

            for (auto& p : x.following) {
                if (p != who) continue;
                containsUser = true;
            }

            // Check to make sure the user doesn't already exist in our list.
            eosio_assert(!containsUser, "Already following user.");

            // Add the user to our list.
            x.following.push_back(who);
        });
    }

    void Thoughts::delfollower( account_name account, account_name who, string memo ) {
        require_auth( account );

        // Find the user.
        auto user = _users.find(account);
        eosio_assert(user != _users.end(), "User not found.");

        // Modify the user and remove the person they are following.
        _users.modify(user, account, [&](auto& x) {
            bool containsUser = false;
            account_name reference;

            // Check if the list contains the user or not.
            for (auto& p : x.following) {
                if (p != who) continue;
                containsUser = true;
                reference = p;
            }

            // If the user was not found we'll just exit here.
            eosio_assert(containsUser, "Followed user was not found.");

            // Determine the index position of the user.
            auto index = find(x.following.begin(), x.following.end(), reference) - x.following.begin();

            // Erase that user.
            x.following.erase(x.following.begin() + index);
        });
    }
}