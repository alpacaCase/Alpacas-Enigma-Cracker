# Alpacas-Enigma-Cracker

Inspired by the Computerphile video https://www.youtube.com/watch?v=RzWB5jL5RX0 this project was written to be a fast, multithreaded method of breaking enigma encryption. 

Try it out by encrypting a short message with the enigma emulator; then using the first 15 letters of your message and the _known plaintext attack_ to see if the computer can break it. Or encrypt a longer text (the first portion of Pride and Prejudice is included as "plaintextP&P.txt") and try using the ciphertext only attack to break it.

## Features:
### Engima emulator 
Encode or decode messages by manually selecting settings for an enigma machine. The output is automatically saved as "ciphertext.txt".

### Ciphertext only attack
As seen on Computerphile this method will brute force seperate elements of the key in turn, attempting to optimise the index of coincidence score at each step. For long messages where only a few plug settings have been used this method works extremely well and in these instances the "Fast" search setting is usually sufficient. However for short messages or ones with lots of plugs this method will struggle as the index of coincidence scores for the correct settings is harder to distinguish from noise. For marginal cases using one of the more thorough search settings such as "Standard" or "Two rounds" can make the difference.

### Known plaintext attack
For instances where brute force won't work, this method (mathematically the same as the one used by the Allies in World War 2) offers another path forward, just as long as you can guess some plaintext! You will first be prompted to find the location of your plaintext in the ciphertext, though this often isn't as hard as it might sound since the enigma machine cannot encipher a letter as itself. Then the computer will construct a graph with plaintext and ciphertext letters connected to one another and look for circuits. These circuits are independent of the plugboard setting allowing the computer to brute force the smaller search space that is just the rotor arrangments and starting positions. The computer will then attempt to improve these settings to give the result that looks the most like english text, hopefully giving you your full message.

Finding the right length plaintext is key for this method to achieve results and is unfortunately not an exact science. Plaintext that is too short will not generate enough/long enough circuits but plaintext that is too long likely has a second rotor turnover at some point in the message which requires running in "full search" mode taking 26x as long. In my experience 15-20 characters is usually the ideal, though if you are lucky it can be much less.

## Known issues:
When working with long ciphertext in the _known plaintext attack_ the ciphertext can move onto new lines making positioning the plaintext more difficult.
