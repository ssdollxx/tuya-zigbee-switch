import argparse
from jinja2 import Environment, FileSystemLoader, select_autoescape
from pathlib import Path
import yaml


env = Environment(
    loader=FileSystemLoader("helper_scripts/templates"),
    autoescape=select_autoescape(),
    trim_blocks=True,
    lstrip_blocks=True,
)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Create Zigbee2mqtt converter for custom devices",
        epilog="Generates a js file that adds support of re-flashed devices to z2m",
    )
    parser.add_argument(
        "db_file", metavar="INPUT", type=str, help="File with device db"
    )
    parser.add_argument(
        "--z2m-v1", action=argparse.BooleanOptionalAction, help="Use old z2m"
    )

    args = parser.parse_args()

    db_str = Path(args.db_file).read_text()
    db = yaml.safe_load(db_str)

    devices = []

    for device in db.values():
        config = device["config_str"]
        zb_manufacturer, zb_model, *peripherals = config.rstrip(";").split(";")

        relay_cnt = 0
        switch_cnt = 0
        indicators_cnt = 0
        for peripheral in peripherals:
            if peripheral[0] == "R":
                relay_cnt += 1
            if peripheral[0] == 'S':
                switch_cnt += 1
            if peripheral[0] == 'I':
                indicators_cnt += 1
            
        if relay_cnt == 1:
            relay_names = ["relay"]
        elif relay_cnt == 2:
            relay_names = ["left", "right"]
        elif relay_cnt == 3:
            relay_names = ["left", "middle", "right"]
        else:
            relay_names = [f"relay_{index}" for index in range(relay_cnt)]

        devices.append({
            "zb_models": [zb_model, *device.get("old_zb_models", [])],
            "model": device["name"],
            "switchNames": [str(index + 1) for index in range(switch_cnt)],
            "relayNames": relay_names,
            "relayIndicatorNames": relay_names[:indicators_cnt],
        })

    template = env.get_template("switch_custom.js.jinja")

    print(template.render(devices=devices, z2m_v1=args.z2m_v1))

    exit(0)

    

