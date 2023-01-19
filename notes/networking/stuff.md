### Ethernet Frame
An ethernet frame is a data-link layer protocol data unit. It is made up of the following components:

- `Preamble`: Ethernet frame starts with 7-byte preamble which is a pattern of alternative 0's and 1's which indicates starting of the frame and allows sender and receiver to establish bit synchronization.

- `Start of frame delimiter`: This is a 1-byte field which is always set to 10101011.
- `Destination address`: 6-byte field which stores the MAC address of the destination.
- `Source address`: 6-byte field which stores the MAC address of the source machine.
- `Length`: 2-byte field that indicates the length of the entire ethernet frame.
- `Data`: The actual data is present here (known as payload). The size of data should be in the range 46-1500 bytes.
- `Cyclic Redundancy Check (CRC)`: A 4-byte field that contains 32-bit hash code of data, which is generated over the destination address, source address, length, and data field. If the checksum computed by destination is not the same as the sent value, data received is corrupted.