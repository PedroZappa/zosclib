# zosc
zosc: OSC C++ Lib

## TODO
- [ ] OSC Address Parsing
- [ ] Messagte Type Handling
- [ ] OSC Type Tagging
- [ ] Network Layer Transport (UDP Support)
- [ ] Bundle Scheduling and Handling
> void schedule(const ZoscBundle &bundle);
> void sendScheduledBundles();
- [ ] Error Handling & Validation
- [ ] SCheck Serialization/Deserialization Edge Cases
- [ ] OSC TimeTag Handling
- [ ] Unit Tests
- [x] ZoscSender (send over UDP)
    * Store Adress/port for the recipient
    * Methods to send Messages & Bundles
    * UDP Socket management
- [ ] ZoscReceiver
    * Listen on a UDP port
    * Deserialize Messages & Bundles
    * Process & Handle incoming data
    * Notify of message reception

## Advances TODOs
- [ ] OscTypeTagger
- [ ] OscAddressParser
- [ ] OscAddressMatcher
- [ ] OscPacket
- [ ] OscServer
