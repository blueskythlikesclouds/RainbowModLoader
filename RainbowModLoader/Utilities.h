﻿#pragma once

inline void processDirectoryPaths(std::vector<std::string>& directoryPaths, const bool reverse)
{
    std::vector<std::string> newDirectoryPaths;

    const std::filesystem::path currentPath = std::filesystem::current_path().lexically_normal();

    for (auto& directoryPath : directoryPaths)
    {
        const std::filesystem::path path = std::filesystem::path(directoryPath).lexically_normal();

        if (!std::filesystem::is_directory(path))
            continue;

        const std::string newDirectoryPath = path.u8string();
        const std::string relativePath = std::filesystem::relative(path, currentPath).u8string();

        std::string newPath = !relativePath.empty() && relativePath.size() < newDirectoryPath.size() ? relativePath : newDirectoryPath;

        std::replace(newPath.begin(), newPath.end(), '\\', '/');

        if (!newPath.empty() && newPath.back() == '/')
            newPath.pop_back();

        newDirectoryPaths.push_back(newPath);
    }

    if (reverse)
        std::reverse(newDirectoryPaths.begin(), newDirectoryPaths.end());

    std::swap(directoryPaths, newDirectoryPaths);
}

inline void processFilePaths(std::vector<std::wstring>& filePaths, const bool reverse)
{
    std::vector<std::wstring> newFilePaths;

    for (auto& filePath : filePaths)
    {
        if (!std::filesystem::is_regular_file(filePath))
            continue;

        std::wstring newPath = std::filesystem::absolute(filePath).lexically_normal().wstring();
        std::replace(newPath.begin(), newPath.end(), L'\\', L'/');

        newFilePaths.push_back(newPath);
    }

    if (reverse)
        std::reverse(newFilePaths.begin(), newFilePaths.end());

    std::swap(filePaths, newFilePaths);
}

inline std::wstring convertMultiByteToWideChar(const std::string& value)
{
    WCHAR wideChar[0x400];
    MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wideChar, _countof(wideChar));

    return std::wstring(wideChar);
}

inline std::string getDirectoryPath(const std::string& path)
{
    std::string directoryPath;

    const size_t index = path.find_last_of("\\/");
    if (index != std::string::npos)
        directoryPath = path.substr(0, index + 1);

    return directoryPath;
}