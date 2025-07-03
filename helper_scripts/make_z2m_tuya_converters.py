import argparse
from jinja2 import Environment, FileSystemLoader, select_autoescape
from pathlib import Path
import yaml

env = Environment(
    loader=FileSystemLoader("helper_scripts/templates"),
    autoescape=select_autoescape(),
    trim_blocks=True,
    lstrip_blocks=True
)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create Zigbee2mqtt converter for tuya devices with ota",
        epilog="Generates a js file that adds ota support for given tuya models")
    parser.add_argument(
        "db_file", metavar="INPUT", type=str, help="File with device db"
    )
    parser.add_argument(
        "--z2m-v1", action=argparse.BooleanOptionalAction, help="Use old z2m"
    )


    args = parser.parse_args()

    db_str = Path(args.db_file).read_text()
    db = yaml.safe_load(db_str)

    tuyaModels = [
        entry["stock_converter_model"]
        for entry in db.values()
        if entry.get("stock_converter_manufacturer", "tuya") == "tuya"
    ]
    moesModels = [
        entry["stock_converter_model"]
        for entry in db.values()
        if entry.get("stock_converter_manufacturer", "tuya") == "moes"
    ]
    avattoModels = [
        entry["stock_converter_model"]
        for entry in db.values()
        if entry.get("stock_converter_manufacturer", "tuya") == "avatto"
    ]

    template = env.get_template("tuya_with_ota.js.jinja")

    print(template.render(
        tuyaModels=sorted(list(set(tuyaModels))),
        moesModels=sorted(list(set(moesModels))),
        avattoModels=sorted(list(set(avattoModels))),
         z2m_v1=args.z2m_v1)
    )
   
    exit(0)