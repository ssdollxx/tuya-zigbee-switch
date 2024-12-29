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
        'Tuya-CUSTOM'
    ],
    "TS0012_END_DEVICE": [
         '_TZ3000_jl7qyupf',
        '_TZ3000_nPGIPl5D',
        '_TZ3000_kpatq5pq',
        '_TZ3000_ljhbw1c9',
        '_TZ3000_4zf0crgo',
        'Tuya-CUSTOM'
    ]
}


def make_ota_index_entry(file: Path, base_url: str, board: str | None) -> dict[str, str | int]:
    data = file.read_bytes()
    res = {
        "fileName": file.name,
        "fileVersion": int.from_bytes(data[14:18], "little"),
        "fileSize": len(data),
        
        "url": f"{base_url}/{file}",
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
        epilog="Reads a zigbee image file and updates index.json")
    parser.add_argument("filename", metavar="INPUT", type=str, help="OTA filename")
    parser.add_argument("index_file", type=str, help="OTA index.json file")
    parser.add_argument("--base-url", required=False, help="Base url to use", 
                        default="https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main")
    parser.add_argument("--board", required=False, help="Used to select manufacturerName list to avoid flashing wrong devices")

    args = parser.parse_args()

    entry = make_ota_index_entry(
        file=Path(args.filename),
        base_url=args.base_url,
        board=args.board,
    )
    index_file = Path(args.index_file)
    if index_file.exists():
        index_data = json.loads(index_file.read_text())
    else:
        index_data = []
    index_data = [
        it for it in index_data if
        (
            it.get("manufacturerName") != entry.get("manufacturerName")
            or it["manufacturerCode"] != entry["manufacturerCode"]    
            or it["imageType"] != entry["imageType"]
        ) 
    ]
    index_data.append(entry)
    index_file.write_text(json.dumps(
        index_data,
        indent=2,
    ))
    exit(0)