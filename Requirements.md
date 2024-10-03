# Project Requirements

This project is for building a Windows Dynamic Link Library (DLL). To successfully build and run the project, the following tools and SDKs are required:

## 1. Visual Studio 2022

Ensure that you have Visual Studio 2022 installed. You can download it from the official website:

- [Download Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)

### Required Workloads:
During installation, select the following workloads:
- **Desktop development with C++**
  - This will provide the necessary tools for creating and managing Windows DLLs.

### Additional Components:
- **Windows 10 SDK (latest version)**: Ensure that the Windows SDK is selected during installation or manually installed as noted below.

## 2. Latest Windows SDK

The latest version of the Windows SDK is required to build the DLL. You can download it from the official Microsoft website:

- [Download Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)

After installation, confirm that the SDK is correctly integrated with Visual Studio and selected as the active SDK for the project.

## 3. DLL Project-Specific Requirements

- **Platform**: The project is designed to be built for Windows 10 or higher.
- **Target Architecture**: Make sure you are using the correct architecture settings (e.g., x64, x86) as defined in the project's configuration.
- **Build Configuration**: Ensure you use the correct build configuration (`Release` or `Debug`) as defined in the project.

## Additional Information

If any additional third-party libraries or dependencies are required for building the DLL, they will be listed in the `README.md` or provided in the project's documentation.

For any issues related to the setup or installation, please refer to the official documentation or open an issue in this repository.

