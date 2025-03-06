import argparse
from jinja2 import Environment, FileSystemLoader, select_autoescape

env = Environment(
    loader=FileSystemLoader("helper_scripts/templates"),
    autoescape=select_autoescape(),
    trim_blocks=True,
    lstrip_blocks=True
)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create Zigbee2mqtt converter for tuya devices with ota",
        epilog="Generates a js file that adds ota support for given tuya models")
    parser.add_argument("models", metavar="INPUT", nargs='+', type=str, help="Model names")
    parser.add_argument(
        "--z2m-v1", action=argparse.BooleanOptionalAction, help="Use old z2m"
    )


    args = parser.parse_args()

    template = env.get_template("tuya_with_ota.js.jinja")

    print(template.render(models=sorted(list(set(args.models))), z2m_v1=args.z2m_v1))
   
    exit(0)