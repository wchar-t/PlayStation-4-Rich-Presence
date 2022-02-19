# PlayStation 4 Rich Presence

PS4RP is a Discord Rich Presence implementation for the PlayStation 4. It is compiled having a 9.00FW as target, but should work for other firmwares - the code itself wouldn't change at all. 

It can be ran on mobile, as it can run python, but i do not recommend

![image](https://user-images.githubusercontent.com/8273763/154806780-0f65c39d-22e2-444c-af40-426d08d9b231.png)
![image](https://user-images.githubusercontent.com/8273763/154806791-b0e70044-faf5-4c98-8962-ece7d06c0501.png)


## Compilation

1. Setup the [libPS4](https://github.com/Scene-Collective/ps4-payload-sdk)
2. You may need to patch the lib to include sceKernelGetdents(), as i did
3. Simply run `make` on /ps4/payload

## Usage

1. Place you Discord token in tokens/token.0.txt
2. Change PS4_API in main.py to your console's ip
3. Run `pip3 install aiohttp`
4. Run `python3 main.py`
5. Send payload.bin to the console

* I suggest playing around with ps4_api.py, discord_gateway.py and cusa.txt, so you are able to use your own assets
