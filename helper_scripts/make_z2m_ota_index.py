import argparse
import hashlib
import json
from pathlib import Path


def make_ota_index_entry(file: Path, base_url: str) -> dict[str, str | int]:
    data = file.read_bytes()
    return {
        "fileName": file.name,
        "fileVersion": int.from_bytes(data[14:18], "little"),
        "fileSize": len(data),
        
        "url": f"{base_url}/{file.name}",
        "imageType": int.from_bytes(data[12:14], "little"),
        "manufacturerCode": int.from_bytes(data[10:12], "little"),
        "sha512": hashlib.sha512(data).hexdigest(),
        "otaHeaderString": data[20:52].decode('unicode_escape'),
    }



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create Zigbee2mqtt index json",
        epilog="Reads a zigbee image file and outputs an index.json on standard output")
    parser.add_argument("filename", metavar="INPUT", type=str, help="OTA filename")
    parser.add_argument("--base-url", required=False, help="Base url to use", 
                        default="https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/bin/")

    args = parser.parse_args()

    entry = make_ota_index_entry(
        file=Path(args.filename),
        base_url=args.base_url,
    )
    print(json.dumps(
        [entry],
        indent=2,
    ))
    exit(0)