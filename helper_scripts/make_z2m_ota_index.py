import argparse
import hashlib
import json
from pathlib import Path

BOARD_TO_MANUFACTURER_NAMES = {
    "TS0012": [
         '_TZ3000_jl7qyupf',
        '_TZ3000_nPGIPl5D',
        '_TZ3000_kpatq5pq',
        '_TZ3000_ljhbw1c9',
        '_TZ3000_4zf0crgo',
    ]
}


def make_ota_index_entry(file: Path, base_url: str, board: str | None) -> dict[str, str | int]:
    data = file.read_bytes()
    res = {
        "fileName": file.name,
        "fileVersion": int.from_bytes(data[14:18], "little"),
        "fileSize": len(data),
        
        "url": f"{base_url}/{file.name}",
        "imageType": int.from_bytes(data[12:14], "little"),
        "manufacturerCode": int.from_bytes(data[10:12], "little"),
        "sha512": hashlib.sha512(data).hexdigest(),
        "otaHeaderString": data[20:52].decode('unicode_escape'), 
    }
    if board and board in BOARD_TO_MANUFACTURER_NAMES:
        res["manufacturerName"] = BOARD_TO_MANUFACTURER_NAMES[board]
    return res


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create Zigbee2mqtt index json",
        epilog="Reads a zigbee image file and outputs an index.json on standard output")
    parser.add_argument("filename", metavar="INPUT", type=str, help="OTA filename")
    parser.add_argument("--base-url", required=False, help="Base url to use", 
                        default="https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/bin/")
    parser.add_argument("--board", required=False, help="Used to select manufacturerName list to avoid flashing wrong devices")

    args = parser.parse_args()

    entry = make_ota_index_entry(
        file=Path(args.filename),
        base_url=args.base_url,
        board=args.board,
    )
    print(json.dumps(
        [entry],
        indent=2,
    ))
    exit(0)