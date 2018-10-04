# EOS Thoughts
EOS Smart Contract that acts as a simple twitter on the blockchain.

### Basic Idea and Use Case
Can be used to publish messages that cannot be deleted. Only the user who owns the messages can delete them.

* Create messages as a user.
* Delete messages as a user.
* Follow someone as a user.
* Unfollow someone as a user.

#### Using the Contract
(Anything in 'camelCasing' is a variable)

ie. accountName = YOUR ACCOUNT NAME

#### Get the Users Table
```
cleos get table accountName accountName users
```

#### Setup and Create a Message
This also creates a new account automatically the first time the user publishes a message.

Action:
* create

Properties:
* account
* message
* memo
```
cleos push action accountName create '["userAccount", "Hello World!", "Published a message."]' -p userAccount
```

#### Remove a Message by Key
Action:
* remove

Properties:
* account
* key
* memo
```
cleos push action accountName remove '["userAccount", 1, "Removed a message."]' -p userAccount
```

### Add a Follower to an Account
Action:
* addfollower

Properties:
* account
* who
* memo
```
cleos push action accountName addfollower '["userAccount", "otherAccount", "userAccount is now Following otherAccount"]' -p userAccount
```

### Remove a Follower to an Account
Action:
* delfollower

Properties:
* account
* who
* memo

```
cleos push action accountName delfollower '["userAccount", "otherAccount", "userAccount unfollowed otherAccount"]' -p userAccount
```

