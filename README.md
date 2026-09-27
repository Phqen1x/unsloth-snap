# Unsloth Snap

Snap packaging for the [Unsloth](https://unsloth.ai/) Desktop App (`unsloth-desktop`).

Unsloth is a platform for fast and memory-efficient fine-tuning, training, quantization, and local inference of Large Language Models (LLMs).

## Installation

Install from the Snap Store:

```bash
sudo snap install unsloth-desktop
```

### Recommended Interface Connections

To enable full hardware introspection (GPU detection and system monitoring), connect the following interfaces:

```bash
# Hardware and system monitoring for GPU and CPU metrics
sudo snap connect unsloth-desktop:hardware-observe
sudo snap connect unsloth-desktop:system-observe
sudo snap connect unsloth-desktop:process-control

# Access training datasets or models on external / removable drives
sudo snap connect unsloth-desktop:removable-media
```

*Note: GPU access via `opengl` (providing NVIDIA CUDA runtime access and AMD ROCm `/dev/kfd` / `/dev/dri`) is auto-connected.*

## Building the Snap Locally

Ensure [Snapcraft](https://snapcraft.io/) is installed:

```bash
sudo snap install snapcraft --classic
```

Download the latest upstream `.deb` package or place `unsloth-desktop.deb` in the repository root:

```bash
curl -fL -o unsloth-desktop.deb "https://github.com/unslothai/unsloth/releases/latest/download/Unsloth-Desktop-Ubuntu.deb"
```

Build the snap:

```bash
snapcraft
```

Install the locally built snap:

```bash
sudo snap install --dangerous unsloth-desktop_*.snap
```

## Details & Environment

- **Base**: `core24` (Ubuntu 24.04 LTS)
- **Confinement**: `strict`
- **Extensions**: `gnome` (GNOME 46 platform runtime + WebKitGTK 4.1 layout bindings)
- **Persistent Data**: App state, managed environments, virtual environments, and Hugging Face model caches are stored under `$SNAP_USER_COMMON` (`~/snap/unsloth-desktop/common`) to persist across snap revisions without duplicate copying.
