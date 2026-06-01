Import("env")
import os

project_dir = env.subst("$PROJECT_DIR")

web_files = {
    "INDEX_HTML": os.path.join(project_dir, "web", "index.html"),
    "STYLE_CSS": os.path.join(project_dir, "web", "style.css"),
    "SCRIPT_JS": os.path.join(project_dir, "web", "script.js")
}

generated_dir = os.path.join(project_dir, "src", "generated")
os.makedirs(generated_dir, exist_ok=True)

output = "#pragma once\n\n#include <Arduino.h>\n\n"

for var_name, path in web_files.items():
    with open(path, "r", encoding="utf-8") as f:
        content = f.read()

    output += f'const char {var_name}[] PROGMEM = R"rawliteral(\n'
    output += content
    output += '\n)rawliteral";\n\n'

with open(os.path.join(generated_dir, "WebAssets.h"), "w", encoding="utf-8") as f:
    f.write(output)