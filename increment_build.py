Import("env")

import re
from pathlib import Path

# evita esecuzioni multiple
if globals().get("VERSION_ALREADY_DONE"):
    Return()

VERSION_ALREADY_DONE = True

version_file = Path("include/version.h")

content = version_file.read_text()

match = re.search(
    r'#define FW_VERSION_STRING "(\d+)\.(\d+)\.(\d+)"',
    content
)

if not match:
    print("Versione non trovata")
    Return()

major = int(match.group(1))
minor = int(match.group(2))
patch = int(match.group(3))

# incrementa
patch += 1

# rollover opzionale
if patch >= 100:
    patch = 0
    minor += 1

if minor >= 10:
    minor = 0
    major += 1

new_version = f"{major}.{minor}.{patch}"

new_content = re.sub(
    r'#define FW_VERSION_STRING "(\d+)\.(\d+)\.(\d+)"',
    f'#define FW_VERSION_STRING "{new_version}"',
    content
)

new_content = re.sub(
    r'#define FW_VERSION_MAJOR \d+',
    f'#define FW_VERSION_MAJOR {major}',
    new_content
)

new_content = re.sub(
    r'#define FW_VERSION_MINOR \d+',
    f'#define FW_VERSION_MINOR {minor}',
    new_content
)

new_content = re.sub(
    r'#define FW_VERSION_PATCH \d+',
    f'#define FW_VERSION_PATCH {patch}',
    new_content
)

version_file.write_text(new_content)

print("====================================")
print(f"Nuova versione: {new_version}")
print("====================================")