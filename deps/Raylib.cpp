///
/// Raylib.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#define RL_TEXT_IMPLEMENTATION
#define RAYLIB_NUKLEAR_IMPLEMENTATION

#include <raylib.h>

#include "Raylib.hpp"

namespace ray
{
	void InitWindow(int width, int height, const char* title)
	{
		::InitWindow(width, height, title);
	}

	void CloseWindow(void)
	{
		::CloseWindow();
	}

	bool WindowShouldClose(void)
	{
		return ::WindowShouldClose();
	}

	bool IsWindowReady(void)
	{
		return ::IsWindowReady();
	}

	bool IsWindowFullscreen(void)
	{
		return ::IsWindowFullscreen();
	}

	bool IsWindowHidden(void)
	{
		return ::IsWindowHidden();
	}

	bool IsWindowMinimized(void)
	{
		return ::IsWindowMinimized();
	}

	bool IsWindowMaximized(void)
	{
		return ::IsWindowMaximized();
	}

	bool IsWindowFocused(void)
	{
		return ::IsWindowFocused();
	}

	bool IsWindowResized(void)
	{
		return ::IsWindowResized();
	}

	bool IsWindowState(unsigned int flag)
	{
		return ::IsWindowState(flag);
	}

	void SetWindowState(unsigned int flags)
	{
		::SetWindowState(flags);
	}

	void ClearWindowState(unsigned int flags)
	{
		::ClearWindowState(flags);
	}

	void ToggleFullscreen(void)
	{
		::ToggleFullscreen();
	}

	void ToggleBorderlessWindowed(void)
	{
		::ToggleBorderlessWindowed();
	}

	void MaximizeWindow(void)
	{
		::MaximizeWindow();
	}

	void MinimizeWindow(void)
	{
		::MinimizeWindow();
	}

	void RestoreWindow(void)
	{
		::RestoreWindow();
	}

	void SetWindowIcon(Image image)
	{
		::SetWindowIcon(reinterpret_cast<::Image&>(image));
	}

	void SetWindowIcons(Image* images, int count)
	{
		::SetWindowIcons(reinterpret_cast<::Image*>(images), count);
	}

	void SetWindowTitle(const char* title)
	{
		::SetWindowTitle(title);
	}

	void SetWindowPosition(int x, int y)
	{
		::SetWindowPosition(x, y);
	}

	void SetWindowMonitor(int monitor)
	{
		::SetWindowMonitor(monitor);
	}

	void SetWindowMinSize(int width, int height)
	{
		::SetWindowMinSize(width, height);
	}

	void SetWindowMaxSize(int width, int height)
	{
		::SetWindowMaxSize(width, height);
	}

	void SetWindowSize(int width, int height)
	{
		::SetWindowSize(width, height);
	}

	void SetWindowOpacity(float opacity)
	{
		::SetWindowOpacity(opacity);
	}

	void SetWindowFocused(void)
	{
		::SetWindowFocused();
	}

	void* GetWindowHandle(void)
	{
		return ::GetWindowHandle();
	}

	int GetScreenWidth(void)
	{
		return ::GetScreenWidth();
	}

	int GetScreenHeight(void)
	{
		return ::GetScreenHeight();
	}

	int GetRenderWidth(void)
	{
		return ::GetRenderWidth();
	}

	int GetRenderHeight(void)
	{
		return ::GetRenderHeight();
	}

	int GetMonitorCount(void)
	{
		return ::GetMonitorCount();
	}

	int GetCurrentMonitor(void)
	{
		return ::GetCurrentMonitor();
	}

	Vector2 GetMonitorPosition(int monitor)
	{
		::Vector2 temp = ::GetMonitorPosition(monitor);
		return reinterpret_cast<Vector2&>(temp);
	}

	int GetMonitorWidth(int monitor)
	{
		return ::GetMonitorWidth(monitor);
	}

	int GetMonitorHeight(int monitor)
	{
		return ::GetMonitorHeight(monitor);
	}

	int GetMonitorPhysicalWidth(int monitor)
	{
		return ::GetMonitorPhysicalWidth(monitor);
	}

	int GetMonitorPhysicalHeight(int monitor)
	{
		return ::GetMonitorPhysicalHeight(monitor);
	}

	int GetMonitorRefreshRate(int monitor)
	{
		return ::GetMonitorRefreshRate(monitor);
	}

	Vector2 GetWindowPosition(void)
	{
		::Vector2 temp = ::GetWindowPosition();
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetWindowScaleDPI(void)
	{
		::Vector2 temp = ::GetWindowScaleDPI();
		return reinterpret_cast<Vector2&>(temp);
	}

	const char* GetMonitorName(int monitor)
	{
		return ::GetMonitorName(monitor);
	}

	void SetClipboardText(const char* text)
	{
		::SetClipboardText(text);
	}

	const char* GetClipboardText(void)
	{
		return ::GetClipboardText();
	}

	Image GetClipboardImage(void)
	{
		::Image temp = ::GetClipboardImage();
		return reinterpret_cast<Image&>(temp);
	}

	void EnableEventWaiting(void)
	{
		::EnableEventWaiting();
	}

	void DisableEventWaiting(void)
	{
		::DisableEventWaiting();
	}

	void ShowCursor(void)
	{
		::ShowCursor();
	}

	void HideCursor(void)
	{
		::HideCursor();
	}

	bool IsCursorHidden(void)
	{
		return ::IsCursorHidden();
	}

	void EnableCursor(void)
	{
		::EnableCursor();
	}

	void DisableCursor(void)
	{
		::DisableCursor();
	}

	bool IsCursorOnScreen(void)
	{
		return ::IsCursorOnScreen();
	}

	void ClearBackground(Color color)
	{
		::ClearBackground(reinterpret_cast<::Color&>(color));
	}

	void BeginDrawing(void)
	{
		::BeginDrawing();
	}

	void EndDrawing(void)
	{
		::EndDrawing();
	}

	void BeginMode2D(Camera2D camera)
	{
		::BeginMode2D(reinterpret_cast<::Camera2D&>(camera));
	}

	void EndMode2D(void)
	{
		::EndMode2D();
	}

	void BeginMode3D(Camera3D camera)
	{
		::BeginMode3D(reinterpret_cast<::Camera3D&>(camera));
	}

	void EndMode3D(void)
	{
		::EndMode3D();
	}

	void BeginTextureMode(RenderTexture2D target)
	{
		::BeginTextureMode(reinterpret_cast<::RenderTexture2D&>(target));
	}

	void EndTextureMode(void)
	{
		::EndTextureMode();
	}

	void BeginShaderMode(Shader shader)
	{
		::BeginShaderMode(reinterpret_cast<::Shader&>(shader));
	}

	void EndShaderMode(void)
	{
		::EndShaderMode();
	}

	void BeginBlendMode(int mode)
	{
		::BeginBlendMode(mode);
	}

	void EndBlendMode(void)
	{
		::EndBlendMode();
	}

	void BeginScissorMode(int x, int y, int width, int height)
	{
		::BeginScissorMode(x, y, width, height);
	}

	void EndScissorMode(void)
	{
		::EndScissorMode();
	}

	void BeginVrStereoMode(VrStereoConfig config)
	{
		::BeginVrStereoMode(reinterpret_cast<::VrStereoConfig&>(config));
	}

	void EndVrStereoMode(void)
	{
		::EndVrStereoMode();
	}

	VrStereoConfig LoadVrStereoConfig(VrDeviceInfo device)
	{
		::VrStereoConfig temp = ::LoadVrStereoConfig(reinterpret_cast<::VrDeviceInfo&>(device));
		return reinterpret_cast<VrStereoConfig&>(temp);
	}

	void UnloadVrStereoConfig(VrStereoConfig config)
	{
		::UnloadVrStereoConfig(reinterpret_cast<::VrStereoConfig&>(config));
	}

	Shader LoadShader(const char* vsFileName, const char* fsFileName)
	{
		::Shader temp = ::LoadShader(vsFileName, fsFileName);
		return reinterpret_cast<Shader&>(temp);
	}

	Shader LoadShaderFromMemory(const char* vsCode, const char* fsCode)
	{
		::Shader temp = ::LoadShaderFromMemory(vsCode, fsCode);
		return reinterpret_cast<Shader&>(temp);
	}

	bool IsShaderValid(Shader shader)
	{
		return ::IsShaderValid(reinterpret_cast<::Shader&>(shader));
	}

	int GetShaderLocation(Shader shader, const char* uniformName)
	{
		return ::GetShaderLocation(reinterpret_cast<::Shader&>(shader), uniformName);
	}

	int GetShaderLocationAttrib(Shader shader, const char* attribName)
	{
		return ::GetShaderLocationAttrib(reinterpret_cast<::Shader&>(shader), attribName);
	}

	void SetShaderValue(Shader shader, int locIndex, const void* value, int uniformType)
	{
		::SetShaderValue(reinterpret_cast<::Shader&>(shader), locIndex, value, uniformType);
	}

	void SetShaderValueV(Shader shader, int locIndex, const void* value, int uniformType, int count)
	{
		::SetShaderValueV(reinterpret_cast<::Shader&>(shader), locIndex, value, uniformType, count);
	}

	void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat)
	{
		::SetShaderValueMatrix(reinterpret_cast<::Shader&>(shader), locIndex, reinterpret_cast<::Matrix&>(mat));
	}

	void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture)
	{
		::SetShaderValueTexture(reinterpret_cast<::Shader&>(shader), locIndex, reinterpret_cast<::Texture2D&>(texture));
	}

	void UnloadShader(Shader shader)
	{
		::UnloadShader(reinterpret_cast<::Shader&>(shader));
	}

	Ray GetScreenToWorldRay(Vector2 position, Camera camera)
	{
		::Ray temp = ::GetScreenToWorldRay(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Camera&>(camera));
		return reinterpret_cast<Ray&>(temp);
	}

	Ray GetScreenToWorldRayEx(Vector2 position, Camera camera, int width, int height)
	{
		::Ray temp = ::GetScreenToWorldRayEx(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Camera&>(camera), width, height);
		return reinterpret_cast<Ray&>(temp);
	}

	Vector2 GetWorldToScreen(Vector3 position, Camera camera)
	{
		::Vector2 temp = ::GetWorldToScreen(reinterpret_cast<::Vector3&>(position), reinterpret_cast<::Camera&>(camera));
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetWorldToScreenEx(Vector3 position, Camera camera, int width, int height)
	{
		::Vector2 temp = ::GetWorldToScreenEx(reinterpret_cast<::Vector3&>(position), reinterpret_cast<::Camera&>(camera), width, height);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera)
	{
		::Vector2 temp = ::GetWorldToScreen2D(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Camera2D&>(camera));
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera)
	{
		::Vector2 temp = ::GetScreenToWorld2D(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Camera2D&>(camera));
		return reinterpret_cast<Vector2&>(temp);
	}

	Matrix GetCameraMatrix(Camera camera)
	{
		::Matrix temp = ::GetCameraMatrix(reinterpret_cast<::Camera&>(camera));
		return reinterpret_cast<Matrix&>(temp);
	}

	Matrix GetCameraMatrix2D(Camera2D camera)
	{
		::Matrix temp = ::GetCameraMatrix2D(reinterpret_cast<::Camera2D&>(camera));
		return reinterpret_cast<Matrix&>(temp);
	}

	void SetTargetFPS(int fps)
	{
		::SetTargetFPS(fps);
	}

	float GetFrameTime(void)
	{
		return ::GetFrameTime();
	}

	double GetTime(void)
	{
		return ::GetTime();
	}

	int GetFPS(void)
	{
		return ::GetFPS();
	}

	void SwapScreenBuffer(void)
	{
		::SwapScreenBuffer();
	}

	void PollInputEvents(void)
	{
		::PollInputEvents();
	}

	void WaitTime(double seconds)
	{
		::WaitTime(seconds);
	}

	void SetRandomSeed(unsigned int seed)
	{
		::SetRandomSeed(seed);
	}

	int GetRandomValue(int min, int max)
	{
		return ::GetRandomValue(min, max);
	}

	int* LoadRandomSequence(unsigned int count, int min, int max)
	{
		return ::LoadRandomSequence(count, min, max);
	}

	void UnloadRandomSequence(int* sequence)
	{
		::UnloadRandomSequence(sequence);
	}

	void TakeScreenshot(const char* fileName)
	{
		::TakeScreenshot(fileName);
	}

	void SetConfigFlags(unsigned int flags)
	{
		::SetConfigFlags(flags);
	}

	void OpenURL(const char* url)
	{
		::OpenURL(url);
	}

	void SetTraceLogLevel(int logLevel)
	{
		::SetTraceLogLevel(logLevel);
	}

	void TraceLog(int logLevel, const char* text, ...)
	{
		::TraceLog(logLevel, text);
	}

	void SetTraceLogCallback(TraceLogCallback callback)
	{
		::SetTraceLogCallback(reinterpret_cast<::TraceLogCallback&>(callback));
	}

	void* MemAlloc(unsigned int size)
	{
		return ::MemAlloc(size);
	}

	void* MemRealloc(void* ptr, unsigned int size)
	{
		return ::MemRealloc(ptr, size);
	}

	void MemFree(void* ptr)
	{
		::MemFree(ptr);
	}

	unsigned char* LoadFileData(const char* fileName, int* dataSize)
	{
		return ::LoadFileData(fileName, dataSize);
	}

	void UnloadFileData(unsigned char* data)
	{
		::UnloadFileData(data);
	}

	bool SaveFileData(const char* fileName, void* data, int dataSize)
	{
		return ::SaveFileData(fileName, data, dataSize);
	}

	bool ExportDataAsCode(const unsigned char* data, int dataSize, const char* fileName)
	{
		return ::ExportDataAsCode(data, dataSize, fileName);
	}

	char* LoadFileText(const char* fileName)
	{
		return ::LoadFileText(fileName);
	}

	void UnloadFileText(char* text)
	{
		::UnloadFileText(text);
	}

	bool SaveFileText(const char* fileName, const char* text)
	{
		return ::SaveFileText(fileName, text);
	}

	void SetLoadFileDataCallback(LoadFileDataCallback callback)
	{
		::SetLoadFileDataCallback(reinterpret_cast<::LoadFileDataCallback&>(callback)); // Set custom file binary data loader
	}

	void SetSaveFileDataCallback(SaveFileDataCallback callback)
	{
		::SetSaveFileDataCallback(reinterpret_cast<::SaveFileDataCallback&>(callback)); // Set custom file binary data saver
	}

	void SetLoadFileTextCallback(LoadFileTextCallback callback)
	{
		::SetLoadFileTextCallback(reinterpret_cast<::LoadFileTextCallback&>(callback)); // Set custom file text data loader
	}

	void SetSaveFileTextCallback(SaveFileTextCallback callback)
	{
		::SetSaveFileTextCallback(reinterpret_cast<::SaveFileTextCallback&>(callback)); // Set custom file text data saver
	}

	int FileRename(const char* fileName, const char* fileRename)
	{
		return ::FileRename(fileName, fileRename);
	}

	int FileRemove(const char* fileName)
	{
		return ::FileRemove(fileName);
	}

	int FileCopy(const char* srcPath, const char* dstPath)
	{
		return ::FileCopy(srcPath, dstPath);
	}

	int FileMove(const char* srcPath, const char* dstPath)
	{
		return ::FileMove(srcPath, dstPath);
	}

	int FileTextReplace(const char* fileName, const char* search, const char* replacement)
	{
		return ::FileTextReplace(fileName, search, replacement);
	}

	int FileTextFindIndex(const char* fileName, const char* search)
	{
		return ::FileTextFindIndex(fileName, search);
	}

	bool FileExists(const char* fileName)
	{
		return ::FileExists(fileName);
	}

	bool DirectoryExists(const char* dirPath)
	{
		return ::DirectoryExists(dirPath);
	}

	bool IsFileExtension(const char* fileName, const char* ext)
	{
		return ::IsFileExtension(fileName, ext);
	}

	int GetFileLength(const char* fileName)
	{
		return ::GetFileLength(fileName);
	}

	long GetFileModTime(const char* fileName)
	{
		return ::GetFileModTime(fileName);
	}

	const char* GetFileExtension(const char* fileName)
	{
		return ::GetFileExtension(fileName);
	}

	const char* GetFileName(const char* filePath)
	{
		return ::GetFileName(filePath);
	}

	const char* GetFileNameWithoutExt(const char* filePath)
	{
		return ::GetFileNameWithoutExt(filePath);
	}

	const char* GetDirectoryPath(const char* filePath)
	{
		return ::GetDirectoryPath(filePath);
	}

	const char* GetPrevDirectoryPath(const char* dirPath)
	{
		return ::GetPrevDirectoryPath(dirPath);
	}

	const char* GetWorkingDirectory(void)
	{
		return ::GetWorkingDirectory();
	}

	const char* GetApplicationDirectory(void)
	{
		return ::GetApplicationDirectory();
	}

	int MakeDirectory(const char* dirPath)
	{
		return ::MakeDirectory(dirPath);
	}

	int ChangeDirectory(const char* dirPath)
	{
		return ::ChangeDirectory(dirPath);
	}

	bool IsPathFile(const char* path)
	{
		return ::IsPathFile(path);
	}

	bool IsFileNameValid(const char* fileName)
	{
		return ::IsFileNameValid(fileName);
	}

	FilePathList LoadDirectoryFiles(const char* dirPath)
	{
		::FilePathList temp = ::LoadDirectoryFiles(dirPath);
		return reinterpret_cast<FilePathList&>(temp);
	}

	FilePathList LoadDirectoryFilesEx(const char* basePath, const char* filter, bool scanSubdirs)
	{
		::FilePathList temp = ::LoadDirectoryFilesEx(basePath, filter, scanSubdirs);
		return reinterpret_cast<FilePathList&>(temp);
	}

	void UnloadDirectoryFiles(FilePathList files)
	{
		::UnloadDirectoryFiles(reinterpret_cast<::FilePathList&>(files));
	}

	bool IsFileDropped(void)
	{
		return ::IsFileDropped();
	}

	FilePathList LoadDroppedFiles(void)
	{
		::FilePathList temp = ::LoadDroppedFiles();
		return reinterpret_cast<FilePathList&>(temp);
	}

	void UnloadDroppedFiles(FilePathList files)
	{
		::UnloadDroppedFiles(reinterpret_cast<::FilePathList&>(files));
	}

	unsigned int GetDirectoryFileCount(const char* dirPath)
	{
		return ::GetDirectoryFileCount(dirPath);
	}

	unsigned int GetDirectoryFileCountEx(const char* basePath, const char* filter, bool scanSubdirs)
	{
		return ::GetDirectoryFileCountEx(basePath, filter, scanSubdirs);
	}

	unsigned char* CompressData(const unsigned char* data, int dataSize, int* compDataSize)
	{
		return ::CompressData(data, dataSize, compDataSize);
	}

	unsigned char* DecompressData(const unsigned char* compData, int compDataSize, int* dataSize)
	{
		return ::DecompressData(compData, compDataSize, dataSize);
	}

	char* EncodeDataBase64(const unsigned char* data, int dataSize, int* outputSize)
	{
		return ::EncodeDataBase64(data, dataSize, outputSize);
	}

	unsigned char* DecodeDataBase64(const char* text, int* outputSize)
	{
		return ::DecodeDataBase64(text, outputSize);
	}

	unsigned int ComputeCRC32(unsigned char* data, int dataSize)
	{
		return ::ComputeCRC32(data, dataSize);
	}

	unsigned int* ComputeMD5(unsigned char* data, int dataSize)
	{
		return ::ComputeMD5(data, dataSize);
	}

	unsigned int* ComputeSHA1(unsigned char* data, int dataSize)
	{
		return ::ComputeSHA1(data, dataSize);
	}

	unsigned int* ComputeSHA256(unsigned char* data, int dataSize)
	{
		return ::ComputeSHA256(data, dataSize);
	}

	AutomationEventList LoadAutomationEventList(const char* fileName)
	{
		::AutomationEventList temp = ::LoadAutomationEventList(fileName);
		return reinterpret_cast<AutomationEventList&>(temp);
	}

	void UnloadAutomationEventList(AutomationEventList list)
	{
		::UnloadAutomationEventList(reinterpret_cast<::AutomationEventList&>(list));
	}

	bool ExportAutomationEventList(AutomationEventList list, const char* fileName)
	{
		return ::ExportAutomationEventList(reinterpret_cast<::AutomationEventList&>(list), fileName);
	}

	void SetAutomationEventList(AutomationEventList* list)
	{
		::SetAutomationEventList(reinterpret_cast<::AutomationEventList*>(list));
	}

	void SetAutomationEventBaseFrame(int frame)
	{
		::SetAutomationEventBaseFrame(frame);
	}

	void StartAutomationEventRecording(void)
	{
		::StartAutomationEventRecording();
	}

	void StopAutomationEventRecording(void)
	{
		::StopAutomationEventRecording();
	}

	void PlayAutomationEvent(AutomationEvent event)
	{
		::PlayAutomationEvent(reinterpret_cast<::AutomationEvent&>(event));
	}

	bool IsKeyPressed(int key)
	{
		return ::IsKeyPressed(key);
	}

	bool IsKeyPressedRepeat(int key)
	{
		return ::IsKeyPressedRepeat(key);
	}

	bool IsKeyDown(int key)
	{
		return ::IsKeyDown(key);
	}

	bool IsKeyReleased(int key)
	{
		return ::IsKeyReleased(key);
	}

	bool IsKeyUp(int key)
	{
		return ::IsKeyUp(key);
	}

	int GetKeyPressed(void)
	{
		return ::GetKeyPressed();
	}

	int GetCharPressed(void)
	{
		return ::GetCharPressed();
	}

	const char* GetKeyName(int key)
	{
		return ::GetKeyName(key);
	}

	void SetExitKey(int key)
	{
		::SetExitKey(key);
	}

	bool IsGamepadAvailable(int gamepad)
	{
		return ::IsGamepadAvailable(gamepad);
	}

	const char* GetGamepadName(int gamepad)
	{
		return ::GetGamepadName(gamepad);
	}

	bool IsGamepadButtonPressed(int gamepad, int button)
	{
		return ::IsGamepadButtonPressed(gamepad, button);
	}

	bool IsGamepadButtonDown(int gamepad, int button)
	{
		return ::IsGamepadButtonDown(gamepad, button);
	}

	bool IsGamepadButtonReleased(int gamepad, int button)
	{
		return ::IsGamepadButtonReleased(gamepad, button);
	}

	bool IsGamepadButtonUp(int gamepad, int button)
	{
		return ::IsGamepadButtonUp(gamepad, button);
	}

	int GetGamepadButtonPressed(void)
	{
		return ::GetGamepadButtonPressed();
	}

	int GetGamepadAxisCount(int gamepad)
	{
		return ::GetGamepadAxisCount(gamepad);
	}

	float GetGamepadAxisMovement(int gamepad, int axis)
	{
		return ::GetGamepadAxisMovement(gamepad, axis);
	}

	int SetGamepadMappings(const char* mappings)
	{
		return ::SetGamepadMappings(mappings);
	}

	void SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration)
	{
		::SetGamepadVibration(gamepad, leftMotor, rightMotor, duration);
	}

	bool IsMouseButtonPressed(int button)
	{
		return ::IsMouseButtonPressed(button);
	}

	bool IsMouseButtonDown(int button)
	{
		return ::IsMouseButtonDown(button);
	}

	bool IsMouseButtonReleased(int button)
	{
		return ::IsMouseButtonReleased(button);
	}

	bool IsMouseButtonUp(int button)
	{
		return ::IsMouseButtonUp(button);
	}

	int GetMouseX(void)
	{
		return ::GetMouseX();
	}

	int GetMouseY(void)
	{
		return ::GetMouseY();
	}

	Vector2 GetMousePosition(void)
	{
		::Vector2 temp = ::GetMousePosition();
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetMouseDelta(void)
	{
		::Vector2 temp = ::GetMouseDelta();
		return reinterpret_cast<Vector2&>(temp);
	}

	void SetMousePosition(int x, int y)
	{
		::SetMousePosition(x, y);
	}

	void SetMouseOffset(int offsetX, int offsetY)
	{
		::SetMouseOffset(offsetX, offsetY);
	}

	void SetMouseScale(float scaleX, float scaleY)
	{
		::SetMouseScale(scaleX, scaleY);
	}

	float GetMouseWheelMove(void)
	{
		return ::GetMouseWheelMove();
	}

	Vector2 GetMouseWheelMoveV(void)
	{
		::Vector2 temp = ::GetMouseWheelMoveV();
		return reinterpret_cast<Vector2&>(temp);
	}

	void SetMouseCursor(int cursor)
	{
		::SetMouseCursor(cursor);
	}

	int GetTouchX(void)
	{
		return ::GetTouchX();
	}

	int GetTouchY(void)
	{
		return ::GetTouchY();
	}

	Vector2 GetTouchPosition(int index)
	{
		::Vector2 temp = ::GetTouchPosition(index);
		return reinterpret_cast<Vector2&>(temp);
	}

	int GetTouchPointId(int index)
	{
		return ::GetTouchPointId(index);
	}

	int GetTouchPointCount(void)
	{
		return ::GetTouchPointCount();
	}

	void SetGesturesEnabled(unsigned int flags)
	{
		::SetGesturesEnabled(flags);
	}

	bool IsGestureDetected(unsigned int gesture)
	{
		return ::IsGestureDetected(gesture);
	}

	int GetGestureDetected(void)
	{
		return ::GetGestureDetected();
	}

	float GetGestureHoldDuration(void)
	{
		return ::GetGestureHoldDuration();
	}

	Vector2 GetGestureDragVector(void)
	{
		::Vector2 temp = ::GetGestureDragVector();
		return reinterpret_cast<Vector2&>(temp);
	}

	float GetGestureDragAngle(void)
	{
		return ::GetGestureDragAngle();
	}

	Vector2 GetGesturePinchVector(void)
	{
		::Vector2 temp = ::GetGesturePinchVector();
		return reinterpret_cast<Vector2&>(temp);
	}

	float GetGesturePinchAngle(void)
	{
		return ::GetGesturePinchAngle();
	}

	void UpdateCamera(Camera* camera, int mode)
	{
		::UpdateCamera(reinterpret_cast<::Camera*>(camera), mode);
	}

	void UpdateCameraPro(Camera* camera, Vector3 movement, Vector3 rotation, float zoom)
	{
		::UpdateCameraPro(reinterpret_cast<::Camera*>(camera), reinterpret_cast<::Vector3&>(movement), reinterpret_cast<::Vector3&>(rotation), zoom);
	}

	void SetShapesTexture(Texture2D texture, Rectangle source)
	{
		::SetShapesTexture(reinterpret_cast<::Texture2D&>(texture), reinterpret_cast<::Rectangle&>(source));
	}

	Texture2D GetShapesTexture(void)
	{
		::Texture2D temp = ::GetShapesTexture();
		return reinterpret_cast<Texture2D&>(temp);
	}

	Rectangle GetShapesTextureRectangle(void)
	{
		::Rectangle temp = ::GetShapesTextureRectangle();
		return reinterpret_cast<Rectangle&>(temp);
	}

	void DrawPixel(int posX, int posY, Color color)
	{
		::DrawPixel(posX, posY, reinterpret_cast<::Color&>(color));
	}

	void DrawPixelV(Vector2 position, Color color)
	{
		::DrawPixelV(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Color&>(color));
	}

	void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color)
	{
		::DrawLine(startPosX, startPosY, endPosX, endPosY, reinterpret_cast<::Color&>(color));
	}

	void DrawLineV(Vector2 startPos, Vector2 endPos, Color color)
	{
		::DrawLineV(reinterpret_cast<::Vector2&>(startPos), reinterpret_cast<::Vector2&>(endPos), reinterpret_cast<::Color&>(color));
	}

	void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color)
	{
		::DrawLineEx(reinterpret_cast<::Vector2&>(startPos), reinterpret_cast<::Vector2&>(endPos), thick, reinterpret_cast<::Color&>(color));
	}

	void DrawLineStrip(const Vector2* points, int pointCount, Color color)
	{
		::DrawLineStrip(reinterpret_cast<const ::Vector2*>(points), pointCount, reinterpret_cast<::Color&>(color));
	}

	void DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color)
	{
		::DrawLineBezier(reinterpret_cast<::Vector2&>(startPos), reinterpret_cast<::Vector2&>(endPos), thick, reinterpret_cast<::Color&>(color));
	}

	void DrawLineDashed(Vector2 startPos, Vector2 endPos, int dashSize, int spaceSize, Color color)
	{
		::DrawLineDashed(reinterpret_cast<::Vector2&>(startPos), reinterpret_cast<::Vector2&>(endPos), dashSize, spaceSize, reinterpret_cast<::Color&>(color));
	}

	void DrawCircle(int centerX, int centerY, float radius, Color color)
	{
		::DrawCircle(centerX, centerY, radius, reinterpret_cast<::Color&>(color));
	}

	void DrawCircleV(Vector2 center, float radius, Color color)
	{
		::DrawCircleV(reinterpret_cast<::Vector2&>(center), radius, reinterpret_cast<::Color&>(color));
	}

	void DrawCircleGradient(Vector2 center, float radius, Color inner, Color outer)
	{
		::DrawCircleGradient(reinterpret_cast<::Vector2&>(center), radius, reinterpret_cast<::Color&>(inner), reinterpret_cast<::Color&>(outer));
	}

	void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color)
	{
		::DrawCircleSector(reinterpret_cast<::Vector2&>(center), radius, startAngle, endAngle, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color)
	{
		::DrawCircleSectorLines(reinterpret_cast<::Vector2&>(center), radius, startAngle, endAngle, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawCircleLines(int centerX, int centerY, float radius, Color color)
	{
		::DrawCircleLines(centerX, centerY, radius, reinterpret_cast<::Color&>(color));
	}

	void DrawCircleLinesV(Vector2 center, float radius, Color color)
	{
		::DrawCircleLinesV(reinterpret_cast<::Vector2&>(center), radius, reinterpret_cast<::Color&>(color));
	}

	void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color)
	{
		::DrawEllipse(centerX, centerY, radiusH, radiusV, reinterpret_cast<::Color&>(color));
	}

	void DrawEllipseV(Vector2 center, float radiusH, float radiusV, Color color)
	{
		::DrawEllipseV(reinterpret_cast<::Vector2&>(center), radiusH, radiusV, reinterpret_cast<::Color&>(color));
	}

	void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color)
	{
		::DrawEllipseLines(centerX, centerY, radiusH, radiusV, reinterpret_cast<::Color&>(color));
	}

	void DrawEllipseLinesV(Vector2 center, float radiusH, float radiusV, Color color)
	{
		::DrawEllipseLinesV(reinterpret_cast<::Vector2&>(center), radiusH, radiusV, reinterpret_cast<::Color&>(color));
	}

	void DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color)
	{
		::DrawRing(reinterpret_cast<::Vector2&>(center), innerRadius, outerRadius, startAngle, endAngle, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color)
	{
		::DrawRingLines(reinterpret_cast<::Vector2&>(center), innerRadius, outerRadius, startAngle, endAngle, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangle(int posX, int posY, int width, int height, Color color)
	{
		::DrawRectangle(posX, posY, width, height, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleV(Vector2 position, Vector2 size, Color color)
	{
		::DrawRectangleV(reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Vector2&>(size), reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleRec(Rectangle rec, Color color)
	{
		::DrawRectangleRec(reinterpret_cast<::Rectangle&>(rec), reinterpret_cast<::Color&>(color));
	}

	void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color)
	{
		::DrawRectanglePro(reinterpret_cast<::Rectangle&>(rec), reinterpret_cast<::Vector2&>(origin), rotation, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom)
	{
		::DrawRectangleGradientV(posX, posY, width, height, reinterpret_cast<::Color&>(top), reinterpret_cast<::Color&>(bottom));
	}

	void DrawRectangleGradientH(int posX, int posY, int width, int height, Color left, Color right)
	{
		::DrawRectangleGradientH(posX, posY, width, height, reinterpret_cast<::Color&>(left), reinterpret_cast<::Color&>(right));
	}

	void DrawRectangleGradientEx(Rectangle rec, Color topLeft, Color bottomLeft, Color bottomRight, Color topRight)
	{
		::DrawRectangleGradientEx(
			reinterpret_cast<::Rectangle&>(rec),
			reinterpret_cast<::Color&>(topLeft),
			reinterpret_cast<::Color&>(bottomLeft),
			reinterpret_cast<::Color&>(bottomRight),
			reinterpret_cast<::Color&>(topRight)
		);
	}

	void DrawRectangleLines(int posX, int posY, int width, int height, Color color)
	{
		::DrawRectangleLines(posX, posY, width, height, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color)
	{
		::DrawRectangleLinesEx(reinterpret_cast<::Rectangle&>(rec), lineThick, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color)
	{
		::DrawRectangleRounded(reinterpret_cast<::Rectangle&>(rec), roundness, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color)
	{
		::DrawRectangleRoundedLines(reinterpret_cast<::Rectangle&>(rec), roundness, segments, reinterpret_cast<::Color&>(color));
	}

	void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color)
	{
		::DrawRectangleRoundedLinesEx(reinterpret_cast<::Rectangle&>(rec), roundness, segments, lineThick, reinterpret_cast<::Color&>(color));
	}

	void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
	{
		::DrawTriangle(reinterpret_cast<::Vector2&>(v1), reinterpret_cast<::Vector2&>(v2), reinterpret_cast<::Vector2&>(v3), reinterpret_cast<::Color&>(color));
	}

	void DrawTriangleGradient(Vector2 v1, Vector2 v2, Vector2 v3, Color c1, Color c2, Color c3)
	{
		::DrawTriangleGradient(
			reinterpret_cast<::Vector2&>(v1),
			reinterpret_cast<::Vector2&>(v2),
			reinterpret_cast<::Vector2&>(v3),
			reinterpret_cast<::Color&>(c1),
			reinterpret_cast<::Color&>(c2),
			reinterpret_cast<::Color&>(c3)
		);
	}

	void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
	{
		::DrawTriangleLines(reinterpret_cast<::Vector2&>(v1), reinterpret_cast<::Vector2&>(v2), reinterpret_cast<::Vector2&>(v3), reinterpret_cast<::Color&>(color));
	}

	void DrawTriangleFan(const Vector2* points, int pointCount, Color color)
	{
		::DrawTriangleFan(reinterpret_cast<const ::Vector2*>(points), pointCount, reinterpret_cast<::Color&>(color));
	}

	void DrawTriangleStrip(const Vector2* points, int pointCount, Color color)
	{
		::DrawTriangleStrip(reinterpret_cast<const ::Vector2*>(points), pointCount, reinterpret_cast<::Color&>(color));
	}

	void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)
	{
		::DrawPoly(reinterpret_cast<::Vector2&>(center), sides, radius, rotation, reinterpret_cast<::Color&>(color));
	}

	void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color)
	{
		::DrawPolyLines(reinterpret_cast<::Vector2&>(center), sides, radius, rotation, reinterpret_cast<::Color&>(color));
	}

	void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color)
	{
		::DrawPolyLinesEx(reinterpret_cast<::Vector2&>(center), sides, radius, rotation, lineThick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineLinear(const Vector2* points, int pointCount, float thick, Color color)
	{
		::DrawSplineLinear(reinterpret_cast<const ::Vector2*>(points), pointCount, thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineBasis(const Vector2* points, int pointCount, float thick, Color color)
	{
		::DrawSplineBasis(reinterpret_cast<const ::Vector2*>(points), pointCount, thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineCatmullRom(const Vector2* points, int pointCount, float thick, Color color)
	{
		::DrawSplineCatmullRom(reinterpret_cast<const ::Vector2*>(points), pointCount, thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineBezierQuadratic(const Vector2* points, int pointCount, float thick, Color color)
	{
		::DrawSplineBezierQuadratic(reinterpret_cast<const ::Vector2*>(points), pointCount, thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineBezierCubic(const Vector2* points, int pointCount, float thick, Color color)
	{
		::DrawSplineBezierCubic(reinterpret_cast<const ::Vector2*>(points), pointCount, thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineSegmentLinear(Vector2 p1, Vector2 p2, float thick, Color color)
	{
		::DrawSplineSegmentLinear(reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(p2), thick, reinterpret_cast<::Color&>(color));
	}

	void DrawSplineSegmentBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color)
	{
		::DrawSplineSegmentBasis(
			reinterpret_cast<::Vector2&>(p1),
			reinterpret_cast<::Vector2&>(p2),
			reinterpret_cast<::Vector2&>(p3),
			reinterpret_cast<::Vector2&>(p4),
			thick,
			reinterpret_cast<::Color&>(color)
		);
	}

	void DrawSplineSegmentCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color)
	{
		::DrawSplineSegmentCatmullRom(
			reinterpret_cast<::Vector2&>(p1),
			reinterpret_cast<::Vector2&>(p2),
			reinterpret_cast<::Vector2&>(p3),
			reinterpret_cast<::Vector2&>(p4),
			thick,
			reinterpret_cast<::Color&>(color)
		);
	}

	void DrawSplineSegmentBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float thick, Color color)
	{
		::DrawSplineSegmentBezierQuadratic(
			reinterpret_cast<::Vector2&>(p1),
			reinterpret_cast<::Vector2&>(c2),
			reinterpret_cast<::Vector2&>(p3),
			thick,
			reinterpret_cast<::Color&>(color)
		);
	}

	void DrawSplineSegmentBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float thick, Color color)
	{
		::DrawSplineSegmentBezierCubic(
			reinterpret_cast<::Vector2&>(p1),
			reinterpret_cast<::Vector2&>(c2),
			reinterpret_cast<::Vector2&>(c3),
			reinterpret_cast<::Vector2&>(p4),
			thick,
			reinterpret_cast<::Color&>(color)
		);
	}

	Vector2 GetSplinePointLinear(Vector2 startPos, Vector2 endPos, float t)
	{
		::Vector2 temp = ::GetSplinePointLinear(reinterpret_cast<::Vector2&>(startPos), reinterpret_cast<::Vector2&>(endPos), t);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetSplinePointBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t)
	{
		::Vector2 temp =
			::GetSplinePointBasis(reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(p2), reinterpret_cast<::Vector2&>(p3), reinterpret_cast<::Vector2&>(p4), t);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetSplinePointCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t)
	{
		::Vector2 temp =
			::GetSplinePointCatmullRom(reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(p2), reinterpret_cast<::Vector2&>(p3), reinterpret_cast<::Vector2&>(p4), t);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetSplinePointBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float t)
	{
		::Vector2 temp = ::GetSplinePointBezierQuadratic(reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(c2), reinterpret_cast<::Vector2&>(p3), t);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 GetSplinePointBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float t)
	{
		::Vector2 temp =
			::GetSplinePointBezierCubic(reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(c2), reinterpret_cast<::Vector2&>(c3), reinterpret_cast<::Vector2&>(p4), t);
		return reinterpret_cast<Vector2&>(temp);
	}

	bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2)
	{
		return ::CheckCollisionRecs(reinterpret_cast<::Rectangle&>(rec1), reinterpret_cast<::Rectangle&>(rec2));
	}

	bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
	{
		return ::CheckCollisionCircles(reinterpret_cast<::Vector2&>(center1), radius1, reinterpret_cast<::Vector2&>(center2), radius2);
	}

	bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec)
	{
		return ::CheckCollisionCircleRec(reinterpret_cast<::Vector2&>(center), radius, reinterpret_cast<::Rectangle&>(rec));
	}

	bool CheckCollisionCircleLine(Vector2 center, float radius, Vector2 p1, Vector2 p2)
	{
		return ::CheckCollisionCircleLine(reinterpret_cast<::Vector2&>(center), radius, reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(p2));
	}

	bool CheckCollisionPointRec(Vector2 point, Rectangle rec)
	{
		return ::CheckCollisionPointRec(reinterpret_cast<::Vector2&>(point), reinterpret_cast<::Rectangle&>(rec));
	}

	bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius)
	{
		return ::CheckCollisionPointCircle(reinterpret_cast<::Vector2&>(point), reinterpret_cast<::Vector2&>(center), radius);
	}

	bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3)
	{
		return ::CheckCollisionPointTriangle(
			reinterpret_cast<::Vector2&>(point),
			reinterpret_cast<::Vector2&>(p1),
			reinterpret_cast<::Vector2&>(p2),
			reinterpret_cast<::Vector2&>(p3)
		);
	}

	bool CheckCollisionPointLine(Vector2 point, Vector2 p1, Vector2 p2, int threshold)
	{
		return ::CheckCollisionPointLine(reinterpret_cast<::Vector2&>(point), reinterpret_cast<::Vector2&>(p1), reinterpret_cast<::Vector2&>(p2), threshold);
	}

	bool CheckCollisionPointPoly(Vector2 point, const Vector2* points, int pointCount)
	{
		return ::CheckCollisionPointPoly(reinterpret_cast<::Vector2&>(point), reinterpret_cast<const ::Vector2*>(points), pointCount);
	}

	bool CheckCollisionLines(Vector2 startPos1, Vector2 endPos1, Vector2 startPos2, Vector2 endPos2, Vector2* collisionPoint)
	{
		return ::CheckCollisionLines(
			reinterpret_cast<::Vector2&>(startPos1),
			reinterpret_cast<::Vector2&>(endPos1),
			reinterpret_cast<::Vector2&>(startPos2),
			reinterpret_cast<::Vector2&>(endPos2),
			reinterpret_cast<::Vector2*>(collisionPoint)
		);
	}

	Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2)
	{
		::Rectangle temp = ::GetCollisionRec(reinterpret_cast<::Rectangle&>(rec1), reinterpret_cast<::Rectangle&>(rec2));
		return reinterpret_cast<Rectangle&>(temp);
	}

	Image LoadImage(const char* fileName)
	{
		::Image temp = ::LoadImage(fileName);
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageRaw(const char* fileName, int width, int height, int format, int headerSize)
	{
		::Image temp = ::LoadImageRaw(fileName, width, height, format, headerSize);
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageAnim(const char* fileName, int* frames)
	{
		::Image temp = ::LoadImageAnim(fileName, frames);
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageAnimFromMemory(const char* fileType, const unsigned char* fileData, int dataSize, int* frames)
	{
		::Image temp = ::LoadImageAnimFromMemory(fileType, fileData, dataSize, frames);
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize)
	{
		::Image temp = ::LoadImageFromMemory(fileType, fileData, dataSize);
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageFromTexture(Texture2D texture)
	{
		::Image temp = ::LoadImageFromTexture(reinterpret_cast<::Texture2D&>(texture));
		return reinterpret_cast<Image&>(temp);
	}

	Image LoadImageFromScreen(void)
	{
		::Image temp = ::LoadImageFromScreen();
		return reinterpret_cast<Image&>(temp);
	}

	bool IsImageValid(Image image)
	{
		return ::IsImageValid(reinterpret_cast<::Image&>(image));
	}

	void UnloadImage(Image image)
	{
		::UnloadImage(reinterpret_cast<::Image&>(image));
	}

	bool ExportImage(Image image, const char* fileName)
	{
		return ::ExportImage(reinterpret_cast<::Image&>(image), fileName);
	}

	unsigned char* ExportImageToMemory(Image image, const char* fileType, int* fileSize)
	{
		return ::ExportImageToMemory(reinterpret_cast<::Image&>(image), fileType, fileSize);
	}

	bool ExportImageAsCode(Image image, const char* fileName)
	{
		return ::ExportImageAsCode(reinterpret_cast<::Image&>(image), fileName);
	}

	Image GenImageColor(int width, int height, Color color)
	{
		::Image temp = ::GenImageColor(width, height, reinterpret_cast<::Color&>(color));
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageGradientLinear(int width, int height, int direction, Color start, Color end)
	{
		::Image temp = ::GenImageGradientLinear(width, height, direction, reinterpret_cast<::Color&>(start), reinterpret_cast<::Color&>(end));
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer)
	{
		::Image temp = ::GenImageGradientRadial(width, height, density, reinterpret_cast<::Color&>(inner), reinterpret_cast<::Color&>(outer));
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageGradientSquare(int width, int height, float density, Color inner, Color outer)
	{
		::Image temp = ::GenImageGradientSquare(width, height, density, reinterpret_cast<::Color&>(inner), reinterpret_cast<::Color&>(outer));
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2)
	{
		::Image temp = ::GenImageChecked(width, height, checksX, checksY, reinterpret_cast<::Color&>(col1), reinterpret_cast<::Color&>(col2));
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageWhiteNoise(int width, int height, float factor)
	{
		::Image temp = ::GenImageWhiteNoise(width, height, factor);
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale)
	{
		::Image temp = ::GenImagePerlinNoise(width, height, offsetX, offsetY, scale);
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageCellular(int width, int height, int tileSize)
	{
		::Image temp = ::GenImageCellular(width, height, tileSize);
		return reinterpret_cast<Image&>(temp);
	}

	Image GenImageText(int width, int height, const char* text)
	{
		::Image temp = ::GenImageText(width, height, text);
		return reinterpret_cast<Image&>(temp);
	}

	Image ImageCopy(Image image)
	{
		::Image temp = ImageCopy(reinterpret_cast<::Image&>(image));
		return reinterpret_cast<Image&>(temp);
	}

	Image ImageFromImage(Image image, Rectangle rec)
	{
		::Image temp = ImageFromImage(reinterpret_cast<::Image&>(image), reinterpret_cast<::Rectangle&>(rec));
		return reinterpret_cast<Image&>(temp);
	}

	Image ImageFromChannel(Image image, int selectedChannel)
	{
		::Image temp = ::ImageFromChannel(reinterpret_cast<::Image&>(image), selectedChannel);
		return reinterpret_cast<Image&>(temp);
	}

	Image ImageText(const char* text, int fontSize, Color color)
	{
		::Image temp = ImageText(text, fontSize, reinterpret_cast<::Color&>(color));
		return reinterpret_cast<Image&>(temp);
	}

	Image ImageTextEx(Font font, const char* text, float fontSize, float spacing, Color tint)
	{
		::Image temp = ImageTextEx(reinterpret_cast<::Font&>(font), text, fontSize, spacing, reinterpret_cast<::Color&>(tint));
		return reinterpret_cast<Image&>(temp);
	}

	void ImageFormat(Image* image, int newFormat)
	{
		::ImageFormat(reinterpret_cast<::Image*>(image), newFormat);
	}

	void ImageToPOT(Image* image, Color fill)
	{
		::ImageToPOT(reinterpret_cast<::Image*>(image), reinterpret_cast<::Color&>(fill));
	}

	void ImageCrop(Image* image, Rectangle crop)
	{
		::ImageCrop(reinterpret_cast<::Image*>(image), reinterpret_cast<::Rectangle&>(crop));
	}

	void ImageAlphaCrop(Image* image, float threshold)
	{
		::ImageAlphaCrop(reinterpret_cast<::Image*>(image), threshold);
	}

	void ImageAlphaClear(Image* image, Color color, float threshold)
	{
		::ImageAlphaClear(reinterpret_cast<::Image*>(image), reinterpret_cast<::Color&>(color), threshold);
	}

	void ImageAlphaMask(Image* image, Image alphaMask)
	{
		::ImageAlphaMask(reinterpret_cast<::Image*>(image), reinterpret_cast<::Image&>(alphaMask));
	}

	void ImageAlphaPremultiply(Image* image)
	{
		::ImageAlphaPremultiply(reinterpret_cast<::Image*>(image));
	}

	void ImageBlurGaussian(Image* image, int blurSize)
	{
		::ImageBlurGaussian(reinterpret_cast<::Image*>(image), blurSize);
	}

	void ImageKernelConvolution(Image* image, const float* kernel, int kernelSize)
	{
		::ImageKernelConvolution(reinterpret_cast<::Image*>(image), kernel, kernelSize);
	}

	void ImageResize(Image* image, int newWidth, int newHeight)
	{
		::ImageResize(reinterpret_cast<::Image*>(image), newWidth, newHeight);
	}

	void ImageResizeNN(Image* image, int newWidth, int newHeight)
	{
		::ImageResizeNN(reinterpret_cast<::Image*>(image), newWidth, newHeight);
	}

	void ImageResizeCanvas(Image* image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill)
	{
		::ImageResizeCanvas(reinterpret_cast<::Image*>(image), newWidth, newHeight, offsetX, offsetY, reinterpret_cast<::Color&>(fill));
	}

	void ImageMipmaps(Image* image)
	{
		::ImageMipmaps(reinterpret_cast<::Image*>(image));
	}

	void ImageDither(Image* image, int rBpp, int gBpp, int bBpp, int aBpp)
	{
		::ImageDither(reinterpret_cast<::Image*>(image), rBpp, gBpp, bBpp, aBpp);
	}

	void ImageFlipVertical(Image* image)
	{
		::ImageFlipVertical(reinterpret_cast<::Image*>(image));
	}

	void ImageFlipHorizontal(Image* image)
	{
		::ImageFlipHorizontal(reinterpret_cast<::Image*>(image));
	}

	void ImageRotate(Image* image, int degrees)
	{
		::ImageRotate(reinterpret_cast<::Image*>(image), degrees);
	}

	void ImageRotateCW(Image* image)
	{
		::ImageRotateCW(reinterpret_cast<::Image*>(image));
	}

	void ImageRotateCCW(Image* image)
	{
		::ImageRotateCCW(reinterpret_cast<::Image*>(image));
	}

	void ImageColorTint(Image* image, Color color)
	{
		::ImageColorTint(reinterpret_cast<::Image*>(image), reinterpret_cast<::Color&>(color));
	}

	void ImageColorInvert(Image* image)
	{
		::ImageColorInvert(reinterpret_cast<::Image*>(image));
	}

	void ImageColorGrayscale(Image* image)
	{
		::ImageColorGrayscale(reinterpret_cast<::Image*>(image));
	}

	void ImageColorContrast(Image* image, int contrast)
	{
		::ImageColorContrast(reinterpret_cast<::Image*>(image), contrast);
	}

	void ImageColorBrightness(Image* image, int brightness)
	{
		::ImageColorBrightness(reinterpret_cast<::Image*>(image), brightness);
	}

	void ImageColorReplace(Image* image, Color color, Color replace)
	{
		::ImageColorReplace(reinterpret_cast<::Image*>(image), reinterpret_cast<::Color&>(color), reinterpret_cast<::Color&>(replace));
	}

	Color* LoadImageColors(Image image)
	{
		::Color* temp = ::LoadImageColors(reinterpret_cast<::Image&>(image));
		return reinterpret_cast<Color*>(temp);
	}

	Color* LoadImagePalette(Image image, int maxPaletteSize, int* colorCount)
	{
		::Color* temp = ::LoadImagePalette(reinterpret_cast<::Image&>(image), maxPaletteSize, colorCount);
		return reinterpret_cast<Color*>(temp);
	}

	void UnloadImageColors(Color* colors)
	{
		::UnloadImageColors(reinterpret_cast<::Color*>(colors));
	}

	void UnloadImagePalette(Color* colors)
	{
		::UnloadImagePalette(reinterpret_cast<::Color*>(colors));
	}

	Rectangle GetImageAlphaBorder(Image image, float threshold)
	{
		::Rectangle temp = ::GetImageAlphaBorder(reinterpret_cast<::Image&>(image), threshold);
		return reinterpret_cast<Rectangle&>(temp);
	}

	Color GetImageColor(Image image, int x, int y)
	{
		::Color temp = ::GetImageColor(reinterpret_cast<::Image&>(image), x, y);
		return reinterpret_cast<Color&>(temp);
	}

	Texture2D LoadTexture(const char* fileName)
	{
		::Texture2D temp = ::LoadTexture(fileName);
		return reinterpret_cast<Texture2D&>(temp);
	}

	Texture2D LoadTextureFromImage(Image image)
	{
		::Texture2D temp = ::LoadTextureFromImage(reinterpret_cast<::Image&>(image));
		return reinterpret_cast<Texture2D&>(temp);
	}

	TextureCubemap LoadTextureCubemap(Image image, int layout)
	{
		::TextureCubemap temp = ::LoadTextureCubemap(reinterpret_cast<::Image&>(image), layout);
		return reinterpret_cast<TextureCubemap&>(temp);
	}

	RenderTexture2D LoadRenderTexture(int width, int height)
	{
		::RenderTexture2D temp = ::LoadRenderTexture(width, height);
		return reinterpret_cast<RenderTexture2D&>(temp);
	}

	bool IsTextureValid(Texture2D texture)
	{
		return ::IsTextureValid(reinterpret_cast<::Texture2D&>(texture));
	}

	void UnloadTexture(Texture2D texture)
	{
		::UnloadTexture(reinterpret_cast<::Texture2D&>(texture));
	}

	bool IsRenderTextureValid(RenderTexture2D target)
	{
		return ::IsRenderTextureValid(reinterpret_cast<::RenderTexture2D&>(target));
	}

	void UnloadRenderTexture(RenderTexture2D target)
	{
		::UnloadRenderTexture(reinterpret_cast<::RenderTexture2D&>(target));
	}

	void UpdateTexture(Texture2D texture, const void* pixels)
	{
		::UpdateTexture(reinterpret_cast<::Texture2D&>(texture), pixels);
	}

	void UpdateTextureRec(Texture2D texture, Rectangle rec, const void* pixels)
	{
		::UpdateTextureRec(reinterpret_cast<::Texture2D&>(texture), reinterpret_cast<::Rectangle&>(rec), pixels);
	}

	void GenTextureMipmaps(Texture2D* texture)
	{
		::GenTextureMipmaps(reinterpret_cast<::Texture2D*>(texture));
	}

	void SetTextureFilter(Texture2D texture, int filter)
	{
		::SetTextureFilter(reinterpret_cast<::Texture2D&>(texture), filter);
	}

	void SetTextureWrap(Texture2D texture, int wrap)
	{
		::SetTextureWrap(reinterpret_cast<::Texture2D&>(texture), wrap);
	}

	void DrawTexture(Texture2D texture, int posX, int posY, Color tint)
	{
		::DrawTexture(reinterpret_cast<::Texture2D&>(texture), posX, posY, reinterpret_cast<::Color&>(tint));
	}

	void DrawTextureV(Texture2D texture, Vector2 position, Color tint)
	{
		::DrawTextureV(reinterpret_cast<::Texture2D&>(texture), reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Color&>(tint));
	}

	void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
	{
		::DrawTextureEx(reinterpret_cast<::Texture2D&>(texture), reinterpret_cast<::Vector2&>(position), rotation, scale, reinterpret_cast<::Color&>(tint));
	}

	void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint)
	{
		::DrawTextureRec(reinterpret_cast<::Texture2D&>(texture), reinterpret_cast<::Rectangle&>(source), reinterpret_cast<::Vector2&>(position), reinterpret_cast<::Color&>(tint));
	}

	void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
	{
		::DrawTexturePro(
			reinterpret_cast<::Texture2D&>(texture),
			reinterpret_cast<::Rectangle&>(source),
			reinterpret_cast<::Rectangle&>(dest),
			reinterpret_cast<::Vector2&>(origin),
			rotation,
			reinterpret_cast<::Color&>(tint)
		);
	}

	void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle dest, Vector2 origin, float rotation, Color tint)
	{
		::DrawTextureNPatch(
			reinterpret_cast<::Texture2D&>(texture),
			reinterpret_cast<::NPatchInfo&>(nPatchInfo),
			reinterpret_cast<::Rectangle&>(dest),
			reinterpret_cast<::Vector2&>(origin),
			rotation,
			reinterpret_cast<::Color&>(tint)
		);
	}

	bool ColorIsEqual(Color col1, Color col2)
	{
		return ::ColorIsEqual(reinterpret_cast<::Color&>(col1), reinterpret_cast<::Color&>(col2));
	}

	Color Fade(Color color, float alpha)
	{
		::Color temp = ::Fade(reinterpret_cast<::Color&>(color), alpha);
		return reinterpret_cast<Color&>(temp);
	}

	int ColorToInt(Color color)
	{
		return ::ColorToInt(reinterpret_cast<::Color&>(color));
	}

	Vector4 ColorNormalize(Color color)
	{
		::Vector4 temp = ::ColorNormalize(reinterpret_cast<::Color&>(color));
		return reinterpret_cast<Vector4&>(temp);
	}

	Color ColorFromNormalized(Vector4 normalized)
	{
		::Color temp = ::ColorFromNormalized(reinterpret_cast<::Vector4&>(normalized));
		return reinterpret_cast<Color&>(temp);
	}

	Vector3 ColorToHSV(Color color)
	{
		::Vector3 temp = ::ColorToHSV(reinterpret_cast<::Color&>(color));
		return reinterpret_cast<Vector3&>(temp);
	}

	Color ColorFromHSV(float hue, float saturation, float value)
	{
		::Color temp = ::ColorFromHSV(hue, saturation, value);
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorTint(Color color, Color tint)
	{
		::Color temp = ::ColorTint(reinterpret_cast<::Color&>(color), reinterpret_cast<::Color&>(tint));
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorBrightness(Color color, float factor)
	{
		::Color temp = ::ColorBrightness(reinterpret_cast<::Color&>(color), factor);
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorContrast(Color color, float contrast)
	{
		::Color temp = ::ColorContrast(reinterpret_cast<::Color&>(color), contrast);
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorAlpha(Color color, float alpha)
	{
		::Color temp = ::ColorAlpha(reinterpret_cast<::Color&>(color), alpha);
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorAlphaBlend(Color dst, Color src, Color tint)
	{
		::Color temp = ::ColorAlphaBlend(reinterpret_cast<::Color&>(dst), reinterpret_cast<::Color&>(src), reinterpret_cast<::Color&>(tint));
		return reinterpret_cast<Color&>(temp);
	}

	Color ColorLerp(Color color1, Color color2, float factor)
	{
		::Color temp = ::ColorLerp(reinterpret_cast<::Color&>(color1), reinterpret_cast<::Color&>(color2), factor);
		return reinterpret_cast<Color&>(temp);
	}

	Color GetColor(unsigned int hexValue)
	{
		::Color temp = ::GetColor(hexValue);
		return reinterpret_cast<Color&>(temp);
	}

	Color GetPixelColor(void* srcPtr, int format)
	{
		::Color temp = ::GetPixelColor(srcPtr, format);
		return reinterpret_cast<Color&>(temp);
	}

	void SetPixelColor(void* dstPtr, Color color, int format)
	{
		::SetPixelColor(dstPtr, reinterpret_cast<::Color&>(color), format);
	}

	int GetPixelDataSize(int width, int height, int format)
	{
		return ::GetPixelDataSize(width, height, format);
	}

	Font GetFontDefault(void)
	{
		::Font temp = ::GetFontDefault();
		return reinterpret_cast<Font&>(temp);
	}

	Font LoadFont(const char* fileName)
	{
		::Font temp = ::LoadFont(fileName);
		return reinterpret_cast<Font&>(temp);
	}

	Font LoadFontEx(const char* fileName, int fontSize, int* codepoints, int codepointCount)
	{
		::Font temp = ::LoadFontEx(fileName, fontSize, codepoints, codepointCount);
		return reinterpret_cast<Font&>(temp);
	}

	Font LoadFontFromImage(Image image, Color key, int firstChar)
	{
		::Font temp = ::LoadFontFromImage(reinterpret_cast<::Image&>(image), reinterpret_cast<::Color&>(key), firstChar);
		return reinterpret_cast<Font&>(temp);
	}

	Font LoadFontFromMemory(const char* fileType, const unsigned char* fileData, int dataSize, int fontSize, int* codepoints, int codepointCount)
	{
		::Font temp = ::LoadFontFromMemory(fileType, fileData, dataSize, fontSize, codepoints, codepointCount);
		return reinterpret_cast<Font&>(temp);
	}

	bool IsFontValid(Font font)
	{
		return ::IsFontValid(reinterpret_cast<::Font&>(font));
	}

	GlyphInfo* LoadFontData(const unsigned char* fileData, int dataSize, int fontSize, const int* codepoints, int codepointCount, int type, int* glyphCount)
	{
		::GlyphInfo* temp = ::LoadFontData(fileData, dataSize, fontSize, codepoints, codepointCount, type, glyphCount);
		return reinterpret_cast<GlyphInfo*>(temp);
	}

	Image GenImageFontAtlas(const GlyphInfo* glyphs, Rectangle** glyphRecs, int glyphCount, int fontSize, int padding, int packMethod)
	{
		::Image temp = ::GenImageFontAtlas(reinterpret_cast<const ::GlyphInfo*>(glyphs), reinterpret_cast<::Rectangle**>(glyphRecs), glyphCount, fontSize, padding, packMethod);
		return reinterpret_cast<Image&>(temp);
	}

	void UnloadFontData(GlyphInfo* glyphs, int glyphCount)
	{
		::UnloadFontData(reinterpret_cast<::GlyphInfo*>(glyphs), glyphCount);
	}

	void UnloadFont(Font font)
	{
		::UnloadFont(reinterpret_cast<::Font&>(font));
	}

	bool ExportFontAsCode(Font font, const char* fileName)
	{
		return ::ExportFontAsCode(reinterpret_cast<::Font&>(font), fileName);
	}

	void DrawFPS(int posX, int posY)
	{
		::DrawFPS(posX, posY);
	}

	void DrawText(const char* text, int posX, int posY, int fontSize, Color color)
	{
		::DrawText(text, posX, posY, fontSize, reinterpret_cast<::Color&>(color));
	}

	void DrawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint)
	{
		::DrawTextEx(reinterpret_cast<::Font&>(font), text, reinterpret_cast<::Vector2&>(position), fontSize, spacing, reinterpret_cast<::Color&>(tint));
	}

	void DrawTextPro(Font font, const char* text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint)
	{
		::DrawTextPro(
			reinterpret_cast<::Font&>(font),
			text,
			reinterpret_cast<::Vector2&>(position),
			reinterpret_cast<::Vector2&>(origin),
			rotation,
			fontSize,
			spacing,
			reinterpret_cast<::Color&>(tint)
		);
	}

	void DrawTextCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint)
	{
		::DrawTextCodepoint(reinterpret_cast<::Font&>(font), codepoint, reinterpret_cast<::Vector2&>(position), fontSize, reinterpret_cast<::Color&>(tint));
	}

	void DrawTextCodepoints(Font font, const int* codepoints, int codepointCount, Vector2 position, float fontSize, float spacing, Color tint)
	{
		::DrawTextCodepoints(
			reinterpret_cast<::Font&>(font),
			codepoints,
			codepointCount,
			reinterpret_cast<::Vector2&>(position),
			fontSize,
			spacing,
			reinterpret_cast<::Color&>(tint)
		);
	}

	void SetTextLineSpacing(int spacing)
	{
		::SetTextLineSpacing(spacing);
	}

	int MeasureText(const char* text, int fontSize)
	{
		return ::MeasureText(text, fontSize);
	}

	Vector2 MeasureTextEx(Font font, const char* text, float fontSize, float spacing)
	{
		::Vector2 temp = ::MeasureTextEx(reinterpret_cast<::Font&>(font), text, fontSize, spacing);
		return reinterpret_cast<Vector2&>(temp);
	}

	Vector2 MeasureTextCodepoints(Font font, const int* codepoints, int length, float fontSize, float spacing)
	{
		::Vector2 temp = ::MeasureTextCodepoints(reinterpret_cast<::Font&>(font), codepoints, length, fontSize, spacing);
		return reinterpret_cast<Vector2&>(temp);
	}

	int GetGlyphIndex(Font font, int codepoint)
	{
		return ::GetGlyphIndex(reinterpret_cast<::Font&>(font), codepoint);
	}

	GlyphInfo GetGlyphInfo(Font font, int codepoint)
	{
		::GlyphInfo temp = ::GetGlyphInfo(reinterpret_cast<::Font&>(font), codepoint);
		return reinterpret_cast<GlyphInfo&>(temp);
	}

	Rectangle GetGlyphAtlasRec(Font font, int codepoint)
	{
		::Rectangle temp = ::GetGlyphAtlasRec(reinterpret_cast<::Font&>(font), codepoint);
		return reinterpret_cast<Rectangle&>(temp);
	}

	char* LoadUTF8(const int* codepoints, int length)
	{
		return ::LoadUTF8(codepoints, length);
	}

	void UnloadUTF8(char* text)
	{
		::UnloadUTF8(text);
	}

	int* LoadCodepoints(const char* text, int* count)
	{
		return ::LoadCodepoints(text, count);
	}

	void UnloadCodepoints(int* codepoints)
	{
		::UnloadCodepoints(codepoints);
	}

	int GetCodepointCount(const char* text)
	{
		return ::GetCodepointCount(text);
	}

	int GetCodepoint(const char* text, int* codepointSize)
	{
		return ::GetCodepoint(text, codepointSize);
	}

	int GetCodepointNext(const char* text, int* codepointSize)
	{
		return ::GetCodepointNext(text, codepointSize);
	}

	int GetCodepointPrevious(const char* text, int* codepointSize)
	{
		return ::GetCodepointPrevious(text, codepointSize);
	}

	const char* CodepointToUTF8(int codepoint, int* utf8Size)
	{
		return ::CodepointToUTF8(codepoint, utf8Size);
	}

	char** LoadTextLines(const char* text, int* count)
	{
		return ::LoadTextLines(text, count);
	}

	void UnloadTextLines(char** text, int lineCount)
	{
		::UnloadTextLines(text, lineCount);
	}

	int TextCopy(char* dst, const char* src)
	{
		return ::TextCopy(dst, src);
	}

	bool TextIsEqual(const char* text1, const char* text2)
	{
		return ::TextIsEqual(text1, text2);
	}

	unsigned int TextLength(const char* text)
	{
		return ::TextLength(text);
	}

	const char* TextFormat(const char* text, ...)
	{
		va_list args;
		va_start(args, text);
		const char* formattedText = ::TextFormat(text, args);
		va_end(args);
		return formattedText;
	}

	const char* TextSubtext(const char* text, int position, int length)
	{
		return ::TextSubtext(text, position, length);
	}

	const char* TextRemoveSpaces(const char* text)
	{
		return ::TextRemoveSpaces(text);
	}

	char* GetTextBetween(const char* text, const char* begin, const char* end)
	{
		return ::GetTextBetween(text, begin, end);
	}

	char* TextReplace(const char* text, const char* search, const char* replacement)
	{
		return ::TextReplace(text, search, replacement);
	}

	char* TextReplaceAlloc(const char* text, const char* search, const char* replacement)
	{
		return ::TextReplaceAlloc(text, search, replacement);
	}

	char* TextReplaceBetween(const char* text, const char* begin, const char* end, const char* replacement)
	{
		return ::TextReplaceBetween(text, begin, end, replacement);
	}

	char* TextReplaceBetweenAlloc(const char* text, const char* begin, const char* end, const char* replacement)
	{
		return ::TextReplaceBetweenAlloc(text, begin, end, replacement);
	}

	char* TextInsert(const char* text, const char* insert, int position)
	{
		return ::TextInsert(text, insert, position);
	}

	char* TextInsertAlloc(const char* text, const char* insert, int position)
	{
		return ::TextInsertAlloc(text, insert, position);
	}

	char* TextJoin(char** textList, int count, const char* delimiter)
	{
		return ::TextJoin(textList, count, delimiter);
	}

	char** TextSplit(const char* text, char delimiter, int* count)
	{
		return ::TextSplit(text, delimiter, count);
	}

	void TextAppend(char* text, const char* append, int* position)
	{
		::TextAppend(text, append, position);
	}

	int TextFindIndex(const char* text, const char* search)
	{
		return ::TextFindIndex(text, search);
	}

	char* TextToUpper(const char* text)
	{
		return ::TextToUpper(text);
	}

	char* TextToLower(const char* text)
	{
		return ::TextToLower(text);
	}

	char* TextToPascal(const char* text)
	{
		return ::TextToPascal(text);
	}

	char* TextToSnake(const char* text)
	{
		return ::TextToSnake(text);
	}

	char* TextToCamel(const char* text)
	{
		return ::TextToCamel(text);
	}

	int TextToInteger(const char* text)
	{
		return ::TextToInteger(text);
	}

	float TextToFloat(const char* text)
	{
		return ::TextToFloat(text);
	}

	void DrawLine3D(Vector3 startPos, Vector3 endPos, Color color)
	{
		::DrawLine3D(reinterpret_cast<::Vector3&>(startPos), reinterpret_cast<::Vector3&>(endPos), reinterpret_cast<::Color&>(color));
	}

	void DrawPoint3D(Vector3 position, Color color)
	{
		::DrawPoint3D(reinterpret_cast<::Vector3&>(position), reinterpret_cast<::Color&>(color));
	}

	void DrawCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color)
	{
		::DrawCircle3D(reinterpret_cast<::Vector3&>(center), radius, reinterpret_cast<::Vector3&>(rotationAxis), rotationAngle, reinterpret_cast<::Color&>(color));
	}

	void DrawTriangle3D(Vector3 v1, Vector3 v2, Vector3 v3, Color color)
	{
		::DrawTriangle3D(reinterpret_cast<::Vector3&>(v1), reinterpret_cast<::Vector3&>(v2), reinterpret_cast<::Vector3&>(v3), reinterpret_cast<::Color&>(color));
	}

	void DrawTriangleStrip3D(Vector3* points, int pointCount, Color color)
	{
		::DrawTriangleStrip3D(reinterpret_cast<::Vector3*>(points), pointCount, reinterpret_cast<::Color&>(color));
	}

	void DrawCube(Vector3 position, float width, float height, float length, Color color)
	{
		::DrawCube(reinterpret_cast<::Vector3&>(position), width, height, length, reinterpret_cast<::Color&>(color));
	}

	void DrawCubeV(Vector3 position, Vector3 size, Color color)
	{
		::DrawCubeV(reinterpret_cast<::Vector3&>(position), reinterpret_cast<::Vector3&>(size), reinterpret_cast<::Color&>(color));
	}

	void DrawCubeWires(Vector3 position, float width, float height, float length, Color color)
	{
		::DrawCubeWires(reinterpret_cast<::Vector3&>(position), width, height, length, reinterpret_cast<::Color&>(color));
	}

	void DrawCubeWiresV(Vector3 position, Vector3 size, Color color)
	{
		::DrawCubeWiresV(reinterpret_cast<::Vector3&>(position), reinterpret_cast<::Vector3&>(size), reinterpret_cast<::Color&>(color));
	}

	void DrawSphere(Vector3 centerPos, float radius, Color color)
	{
		::DrawSphere(reinterpret_cast<::Vector3&>(centerPos), radius, reinterpret_cast<::Color&>(color));
	}

	void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color)
	{
		::DrawSphereEx(reinterpret_cast<::Vector3&>(centerPos), radius, rings, slices, reinterpret_cast<::Color&>(color));
	}

	void DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color)
	{
		::DrawSphereWires(reinterpret_cast<::Vector3&>(centerPos), radius, rings, slices, reinterpret_cast<::Color&>(color));
	}

	void DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color)
	{
		::DrawCylinder(reinterpret_cast<::Vector3&>(position), radiusTop, radiusBottom, height, slices, reinterpret_cast<::Color&>(color));
	}

	void DrawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color)
	{
		::DrawCylinderEx(reinterpret_cast<::Vector3&>(startPos), reinterpret_cast<::Vector3&>(endPos), startRadius, endRadius, sides, reinterpret_cast<::Color&>(color));
	}

	void DrawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color)
	{
		::DrawCylinderWires(reinterpret_cast<::Vector3&>(position), radiusTop, radiusBottom, height, slices, reinterpret_cast<::Color&>(color));
	}

	void DrawCylinderWiresEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color)
	{
		::DrawCylinderWiresEx(reinterpret_cast<::Vector3&>(startPos), reinterpret_cast<::Vector3&>(endPos), startRadius, endRadius, sides, reinterpret_cast<::Color&>(color));
	}

	void DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color)
	{
		::DrawCapsule(reinterpret_cast<::Vector3&>(startPos), reinterpret_cast<::Vector3&>(endPos), radius, slices, rings, reinterpret_cast<::Color&>(color));
	}

	void DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color)
	{
		::DrawCapsuleWires(reinterpret_cast<::Vector3&>(startPos), reinterpret_cast<::Vector3&>(endPos), radius, slices, rings, reinterpret_cast<::Color&>(color));
	}

	void DrawPlane(Vector3 centerPos, Vector2 size, Color color)
	{
		::DrawPlane(reinterpret_cast<::Vector3&>(centerPos), reinterpret_cast<::Vector2&>(size), reinterpret_cast<::Color&>(color));
	}

	void DrawRay(Ray ray, Color color)
	{
		::DrawRay(reinterpret_cast<::Ray&>(ray), reinterpret_cast<::Color&>(color));
	}

	void DrawGrid(int slices, float spacing)
	{
		::DrawGrid(slices, spacing);
	}

	Model LoadModel(const char* fileName)
	{
		::Model temp = ::LoadModel(fileName);
		return reinterpret_cast<Model&>(temp);
	}

	Model LoadModelFromMesh(Mesh mesh)
	{
		::Model temp = ::LoadModelFromMesh(reinterpret_cast<::Mesh&>(mesh));
		return reinterpret_cast<Model&>(temp);
	}

	bool IsModelValid(Model model)
	{
		return ::IsModelValid(reinterpret_cast<::Model&>(model));
	}

	void UnloadModel(Model model)
	{
		::UnloadModel(reinterpret_cast<::Model&>(model));
	}

	BoundingBox GetModelBoundingBox(Model model)
	{
		::BoundingBox temp = ::GetModelBoundingBox(reinterpret_cast<::Model&>(model));
		return reinterpret_cast<BoundingBox&>(temp);
	}

	void DrawModel(Model model, Vector3 position, float scale, Color tint)
	{
		::DrawModel(reinterpret_cast<::Model&>(model), reinterpret_cast<::Vector3&>(position), scale, reinterpret_cast<::Color&>(tint));
	}

	void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint)
	{
		::DrawModelEx(
			reinterpret_cast<::Model&>(model),
			reinterpret_cast<::Vector3&>(position),
			reinterpret_cast<::Vector3&>(rotationAxis),
			rotationAngle,
			reinterpret_cast<::Vector3&>(scale),
			reinterpret_cast<::Color&>(tint)
		);
	}

	void DrawModelWires(Model model, Vector3 position, float scale, Color tint)
	{
		::DrawModelWires(reinterpret_cast<::Model&>(model), reinterpret_cast<::Vector3&>(position), scale, reinterpret_cast<::Color&>(tint));
	}

	void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint)
	{
		::DrawModelWiresEx(
			reinterpret_cast<::Model&>(model),
			reinterpret_cast<::Vector3&>(position),
			reinterpret_cast<::Vector3&>(rotationAxis),
			rotationAngle,
			reinterpret_cast<::Vector3&>(scale),
			reinterpret_cast<::Color&>(tint)
		);
	}

	void DrawBoundingBox(BoundingBox box, Color color)
	{
		::DrawBoundingBox(reinterpret_cast<::BoundingBox&>(box), reinterpret_cast<::Color&>(color));
	}

	void DrawBillboard(Camera camera, Texture2D texture, Vector3 position, float size, Color tint)
	{
		::DrawBillboard(
			reinterpret_cast<::Camera&>(camera),
			reinterpret_cast<::Texture2D&>(texture),
			reinterpret_cast<::Vector3&>(position),
			size,
			reinterpret_cast<::Color&>(tint)
		); // Draw a billboard texture
	}

	void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector2 size, Color tint)
	{
		::DrawBillboardRec(
			reinterpret_cast<::Camera&>(camera),
			reinterpret_cast<::Texture2D&>(texture),
			reinterpret_cast<::Rectangle&>(source),
			reinterpret_cast<::Vector3&>(position),
			reinterpret_cast<::Vector2&>(size),
			reinterpret_cast<::Color&>(tint)
		);
	}

	void DrawBillboardPro(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint)
	{
		::DrawBillboardPro(
			reinterpret_cast<::Camera&>(camera),
			reinterpret_cast<::Texture2D&>(texture),
			reinterpret_cast<::Rectangle&>(source),
			reinterpret_cast<::Vector3&>(position),
			reinterpret_cast<::Vector3&>(up),
			reinterpret_cast<::Vector2&>(size),
			reinterpret_cast<::Vector2&>(origin),
			rotation,
			reinterpret_cast<::Color&>(tint)
		);
	}

	void UploadMesh(Mesh* mesh, bool dynamic)
	{
		::UploadMesh(reinterpret_cast<::Mesh*>(mesh), dynamic);
	}

	void UpdateMeshBuffer(Mesh mesh, int index, const void* data, int dataSize, int offset)
	{
		::UpdateMeshBuffer(reinterpret_cast<::Mesh&>(mesh), index, data, dataSize, offset);
	}

	void UnloadMesh(Mesh mesh)
	{
		::UnloadMesh(reinterpret_cast<::Mesh&>(mesh));
	}

	void DrawMesh(Mesh mesh, Material material, Matrix transform)
	{
		::DrawMesh(reinterpret_cast<::Mesh&>(mesh), reinterpret_cast<::Material&>(material), reinterpret_cast<::Matrix&>(transform));
	}

	void DrawMeshInstanced(Mesh mesh, Material material, const Matrix* transforms, int instances)
	{
		::DrawMeshInstanced(reinterpret_cast<::Mesh&>(mesh), reinterpret_cast<::Material&>(material), reinterpret_cast<const ::Matrix*>(transforms), instances);
	}

	BoundingBox GetMeshBoundingBox(Mesh mesh)
	{
		::BoundingBox temp = ::GetMeshBoundingBox(reinterpret_cast<::Mesh&>(mesh));
		return reinterpret_cast<BoundingBox&>(temp);
	}

	void GenMeshTangents(Mesh* mesh)
	{
		::GenMeshTangents(reinterpret_cast<::Mesh*>(mesh));
	}

	bool ExportMesh(Mesh mesh, const char* fileName)
	{
		return ::ExportMesh(reinterpret_cast<::Mesh&>(mesh), fileName);
	}

	bool ExportMeshAsCode(Mesh mesh, const char* fileName)
	{
		return ::ExportMeshAsCode(reinterpret_cast<::Mesh&>(mesh), fileName);
	}

	Mesh GenMeshPoly(int sides, float radius)
	{
		::Mesh temp = ::GenMeshPoly(sides, radius);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshPlane(float width, float length, int resX, int resZ)
	{
		::Mesh temp = ::GenMeshPlane(width, length, resX, resZ);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshCube(float width, float height, float length)
	{
		::Mesh temp = ::GenMeshCube(width, height, length);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshSphere(float radius, int rings, int slices)
	{
		::Mesh temp = ::GenMeshSphere(radius, rings, slices);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshHemiSphere(float radius, int rings, int slices)
	{
		::Mesh temp = ::GenMeshHemiSphere(radius, rings, slices);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshCylinder(float radius, float height, int slices)
	{
		::Mesh temp = ::GenMeshCylinder(radius, height, slices);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshCone(float radius, float height, int slices)
	{
		::Mesh temp = ::GenMeshCone(radius, height, slices);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshTorus(float radius, float size, int radSeg, int sides)
	{
		::Mesh temp = ::GenMeshTorus(radius, size, radSeg, sides);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshKnot(float radius, float size, int radSeg, int sides)
	{
		::Mesh temp = ::GenMeshKnot(radius, size, radSeg, sides);
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshHeightmap(Image heightmap, Vector3 size)
	{
		::Mesh temp = ::GenMeshHeightmap(reinterpret_cast<::Image&>(heightmap), reinterpret_cast<::Vector3&>(size));
		return reinterpret_cast<Mesh&>(temp);
	}

	Mesh GenMeshCubicmap(Image cubicmap, Vector3 cubeSize)
	{
		::Mesh temp = ::GenMeshCubicmap(reinterpret_cast<::Image&>(cubicmap), reinterpret_cast<::Vector3&>(cubeSize));
		return reinterpret_cast<Mesh&>(temp);
	}

	Material* LoadMaterials(const char* fileName, int* materialCount)
	{
		::Material* temp = ::LoadMaterials(fileName, materialCount);
		return reinterpret_cast<Material*>(temp);
	}

	Material LoadMaterialDefault(void)
	{
		::Material temp = ::LoadMaterialDefault();
		return reinterpret_cast<Material&>(temp);
	}

	bool IsMaterialValid(Material material)
	{
		return ::IsMaterialValid(reinterpret_cast<::Material&>(material));
	}

	void UnloadMaterial(Material material)
	{
		::UnloadMaterial(reinterpret_cast<::Material&>(material));
	}

	void SetMaterialTexture(Material* material, int mapType, Texture2D texture)
	{
		::SetMaterialTexture(reinterpret_cast<::Material*>(material), mapType, reinterpret_cast<::Texture2D&>(texture));
	}

	void SetModelMeshMaterial(Model* model, int meshId, int materialId)
	{
		::SetModelMeshMaterial(reinterpret_cast<::Model*>(model), meshId, materialId);
	}

	ModelAnimation* LoadModelAnimations(const char* fileName, int* animCount)
	{
		::ModelAnimation* temp = ::LoadModelAnimations(fileName, animCount);
		return reinterpret_cast<ModelAnimation*>(temp);
	}

	void UpdateModelAnimation(Model model, ModelAnimation anim, float frame)
	{
		::UpdateModelAnimation(reinterpret_cast<::Model&>(model), reinterpret_cast<::ModelAnimation&>(anim), frame);
	}

	void UpdateModelAnimationEx(Model model, ModelAnimation animA, float frameA, ModelAnimation animB, float frameB, float blend)
	{
		::UpdateModelAnimationEx(reinterpret_cast<::Model&>(model), reinterpret_cast<::ModelAnimation&>(animA), frameA, reinterpret_cast<::ModelAnimation&>(animB), frameB, blend);
	}

	void UnloadModelAnimations(ModelAnimation* animations, int animCount)
	{
		::UnloadModelAnimations(reinterpret_cast<::ModelAnimation*>(animations), animCount);
	}

	bool IsModelAnimationValid(Model model, ModelAnimation anim)
	{
		return ::IsModelAnimationValid(reinterpret_cast<::Model&>(model), reinterpret_cast<::ModelAnimation&>(anim));
	}

	bool CheckCollisionSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2)
	{
		return ::CheckCollisionSpheres(reinterpret_cast<::Vector3&>(center1), radius1, reinterpret_cast<::Vector3&>(center2), radius2);
	}

	bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2)
	{
		return ::CheckCollisionBoxes(reinterpret_cast<::BoundingBox&>(box1), reinterpret_cast<::BoundingBox&>(box2));
	}

	bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center, float radius)
	{
		return ::CheckCollisionBoxSphere(reinterpret_cast<::BoundingBox&>(box), reinterpret_cast<::Vector3&>(center), radius);
	}

	RayCollision GetRayCollisionSphere(Ray ray, Vector3 center, float radius)
	{
		::RayCollision temp = ::GetRayCollisionSphere(reinterpret_cast<::Ray&>(ray), reinterpret_cast<::Vector3&>(center), radius);
		return reinterpret_cast<RayCollision&>(temp);
	}

	RayCollision GetRayCollisionBox(Ray ray, BoundingBox box)
	{
		::RayCollision temp = ::GetRayCollisionBox(reinterpret_cast<::Ray&>(ray), reinterpret_cast<::BoundingBox&>(box));
		return reinterpret_cast<RayCollision&>(temp);
	}

	RayCollision GetRayCollisionMesh(Ray ray, Mesh mesh, Matrix transform)
	{
		::RayCollision temp = ::GetRayCollisionMesh(reinterpret_cast<::Ray&>(ray), reinterpret_cast<::Mesh&>(mesh), reinterpret_cast<::Matrix&>(transform));
		return reinterpret_cast<RayCollision&>(temp);
	}

	RayCollision GetRayCollisionTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3)
	{
		::RayCollision temp =
			::GetRayCollisionTriangle(reinterpret_cast<::Ray&>(ray), reinterpret_cast<::Vector3&>(p1), reinterpret_cast<::Vector3&>(p2), reinterpret_cast<::Vector3&>(p3));
		return reinterpret_cast<RayCollision&>(temp);
	}

	RayCollision GetRayCollisionQuad(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4)
	{
		::RayCollision temp = ::GetRayCollisionQuad(
			reinterpret_cast<::Ray&>(ray),
			reinterpret_cast<::Vector3&>(p1),
			reinterpret_cast<::Vector3&>(p2),
			reinterpret_cast<::Vector3&>(p3),
			reinterpret_cast<::Vector3&>(p4)
		);
		return reinterpret_cast<RayCollision&>(temp);
	}

	void InitAudioDevice(void)
	{
		::InitAudioDevice();
	}

	void CloseAudioDevice(void)
	{
		::CloseAudioDevice();
	}

	bool IsAudioDeviceReady(void)
	{
		return ::IsAudioDeviceReady();
	}

	void SetMasterVolume(float volume)
	{
		::SetMasterVolume(volume);
	}

	float GetMasterVolume(void)
	{
		return ::GetMasterVolume();
	}

	Wave LoadWave(const char* fileName)
	{
		::Wave temp = ::LoadWave(fileName);
		return reinterpret_cast<Wave&>(temp);
	}

	Wave LoadWaveFromMemory(const char* fileType, const unsigned char* fileData, int dataSize)
	{
		::Wave temp = ::LoadWaveFromMemory(fileType, fileData, dataSize);
		return reinterpret_cast<Wave&>(temp);
	}

	bool IsWaveValid(Wave wave)
	{
		return ::IsWaveValid(reinterpret_cast<::Wave&>(wave));
	}

	Sound LoadSound(const char* fileName)
	{
		::Sound temp = ::LoadSound(fileName);
		return reinterpret_cast<Sound&>(temp);
	}

	Sound LoadSoundFromWave(Wave wave)
	{
		::Sound temp = ::LoadSoundFromWave(reinterpret_cast<::Wave&>(wave));
		return reinterpret_cast<Sound&>(temp);
	}

	Sound LoadSoundAlias(Sound source)
	{
		::Sound temp = ::LoadSoundAlias(reinterpret_cast<::Sound&>(source));
		return reinterpret_cast<Sound&>(temp);
	}

	bool IsSoundValid(Sound sound)
	{
		return ::IsSoundValid(reinterpret_cast<::Sound&>(sound));
	}

	void UpdateSound(Sound sound, const void* data, int sampleCount)
	{
		::UpdateSound(reinterpret_cast<::Sound&>(sound), data, sampleCount);
	}

	void UnloadWave(Wave wave)
	{
		::UnloadWave(reinterpret_cast<::Wave&>(wave));
	}

	void UnloadSound(Sound sound)
	{
		::UnloadSound(reinterpret_cast<::Sound&>(sound));
	}

	void UnloadSoundAlias(Sound alias)
	{
		::UnloadSoundAlias(reinterpret_cast<::Sound&>(alias));
	}

	bool ExportWave(Wave wave, const char* fileName)
	{
		return ::ExportWave(reinterpret_cast<::Wave&>(wave), fileName);
	}

	bool ExportWaveAsCode(Wave wave, const char* fileName)
	{
		return ::ExportWaveAsCode(reinterpret_cast<::Wave&>(wave), fileName);
	}

	void PlaySound(Sound sound)
	{
		::PlaySound(reinterpret_cast<::Sound&>(sound));
	}

	void StopSound(Sound sound)
	{
		::StopSound(reinterpret_cast<::Sound&>(sound));
	}

	void PauseSound(Sound sound)
	{
		::PauseSound(reinterpret_cast<::Sound&>(sound));
	}

	void ResumeSound(Sound sound)
	{
		::ResumeSound(reinterpret_cast<::Sound&>(sound));
	}

	bool IsSoundPlaying(Sound sound)
	{
		return ::IsSoundPlaying(reinterpret_cast<::Sound&>(sound));
	}

	void SetSoundVolume(Sound sound, float volume)
	{
		::SetSoundVolume(reinterpret_cast<::Sound&>(sound), volume);
	}

	void SetSoundPitch(Sound sound, float pitch)
	{
		::SetSoundPitch(reinterpret_cast<::Sound&>(sound), pitch);
	}

	void SetSoundPan(Sound sound, float pan)
	{
		::SetSoundPan(reinterpret_cast<::Sound&>(sound), pan);
	}

	Wave WaveCopy(Wave wave)
	{
		::Wave temp = WaveCopy(reinterpret_cast<::Wave&>(wave));
		return reinterpret_cast<Wave&>(temp);
	}

	void WaveCrop(Wave* wave, int initSample, int finalSample)
	{
		::WaveCrop(reinterpret_cast<::Wave*>(wave), initSample, finalSample);
	}

	void WaveFormat(Wave* wave, int sampleRate, int sampleSize, int channels)
	{
		::WaveFormat(reinterpret_cast<::Wave*>(wave), sampleRate, sampleSize, channels);
	}

	float* LoadWaveSamples(Wave wave)
	{
		return ::LoadWaveSamples(reinterpret_cast<::Wave&>(wave));
	}

	void UnloadWaveSamples(float* samples)
	{
		::UnloadWaveSamples(samples);
	}

	Music LoadMusicStream(const char* fileName)
	{
		::Music temp = ::LoadMusicStream(fileName);
		return reinterpret_cast<Music&>(temp);
	}

	Music LoadMusicStreamFromMemory(const char* fileType, const unsigned char* data, int dataSize)
	{
		::Music temp = ::LoadMusicStreamFromMemory(fileType, data, dataSize);
		return reinterpret_cast<Music&>(temp);
	}

	bool IsMusicValid(Music music)
	{
		return ::IsMusicValid(reinterpret_cast<::Music&>(music));
	}

	void UnloadMusicStream(Music music)
	{
		::UnloadMusicStream(reinterpret_cast<::Music&>(music));
	}

	void PlayMusicStream(Music music)
	{
		::PlayMusicStream(reinterpret_cast<::Music&>(music));
	}

	bool IsMusicStreamPlaying(Music music)
	{
		return ::IsMusicStreamPlaying(reinterpret_cast<::Music&>(music));
	}

	void UpdateMusicStream(Music music)
	{
		::UpdateMusicStream(reinterpret_cast<::Music&>(music));
	}

	void StopMusicStream(Music music)
	{
		::StopMusicStream(reinterpret_cast<::Music&>(music));
	}

	void PauseMusicStream(Music music)
	{
		::PauseMusicStream(reinterpret_cast<::Music&>(music));
	}

	void ResumeMusicStream(Music music)
	{
		::ResumeMusicStream(reinterpret_cast<::Music&>(music));
	}

	void SeekMusicStream(Music music, float position)
	{
		::SeekMusicStream(reinterpret_cast<::Music&>(music), position);
	}

	void SetMusicVolume(Music music, float volume)
	{
		::SetMusicVolume(reinterpret_cast<::Music&>(music), volume);
	}

	void SetMusicPitch(Music music, float pitch)
	{
		::SetMusicPitch(reinterpret_cast<::Music&>(music), pitch);
	}

	void SetMusicPan(Music music, float pan)
	{
		::SetMusicPan(reinterpret_cast<::Music&>(music), pan);
	}

	float GetMusicTimeLength(Music music)
	{
		return ::GetMusicTimeLength(reinterpret_cast<::Music&>(music));
	}

	float GetMusicTimePlayed(Music music)
	{
		return ::GetMusicTimePlayed(reinterpret_cast<::Music&>(music));
	}

	AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels)
	{
		::AudioStream temp = ::LoadAudioStream(sampleRate, sampleSize, channels);
		return reinterpret_cast<AudioStream&>(temp);
	}

	bool IsAudioStreamValid(AudioStream stream)
	{
		return ::IsAudioStreamValid(reinterpret_cast<::AudioStream&>(stream));
	}

	void UnloadAudioStream(AudioStream stream)
	{
		::UnloadAudioStream(reinterpret_cast<::AudioStream&>(stream));
	}

	void UpdateAudioStream(AudioStream stream, const void* data, int frameCount)
	{
		::UpdateAudioStream(reinterpret_cast<::AudioStream&>(stream), data, frameCount);
	}

	bool IsAudioStreamProcessed(AudioStream stream)
	{
		return ::IsAudioStreamProcessed(reinterpret_cast<::AudioStream&>(stream));
	}

	void PlayAudioStream(AudioStream stream)
	{
		::PlayAudioStream(reinterpret_cast<::AudioStream&>(stream));
	}

	void PauseAudioStream(AudioStream stream)
	{
		::PauseAudioStream(reinterpret_cast<::AudioStream&>(stream));
	}

	void ResumeAudioStream(AudioStream stream)
	{
		::ResumeAudioStream(reinterpret_cast<::AudioStream&>(stream));
	}

	bool IsAudioStreamPlaying(AudioStream stream)
	{
		return ::IsAudioStreamPlaying(reinterpret_cast<::AudioStream&>(stream));
	}

	void StopAudioStream(AudioStream stream)
	{
		::StopAudioStream(reinterpret_cast<::AudioStream&>(stream));
	}

	void SetAudioStreamVolume(AudioStream stream, float volume)
	{
		::SetAudioStreamVolume(reinterpret_cast<::AudioStream&>(stream), volume);
	}

	void SetAudioStreamPitch(AudioStream stream, float pitch)
	{
		::SetAudioStreamPitch(reinterpret_cast<::AudioStream&>(stream), pitch);
	}

	void SetAudioStreamPan(AudioStream stream, float pan)
	{
		::SetAudioStreamPan(reinterpret_cast<::AudioStream&>(stream), pan);
	}

	void SetAudioStreamBufferSizeDefault(int size)
	{
		::SetAudioStreamBufferSizeDefault(size);
	}

	void SetAudioStreamCallback(AudioStream stream, AudioCallback callback)
	{
		::SetAudioStreamCallback(reinterpret_cast<::AudioStream&>(stream), reinterpret_cast<::AudioCallback&>(callback));
	}

	void AttachAudioStreamProcessor(AudioStream stream, AudioCallback processor)
	{
		::AttachAudioStreamProcessor(reinterpret_cast<::AudioStream&>(stream), reinterpret_cast<::AudioCallback&>(processor));
	}

	void DetachAudioStreamProcessor(AudioStream stream, AudioCallback processor)
	{
		::DetachAudioStreamProcessor(reinterpret_cast<::AudioStream&>(stream), reinterpret_cast<::AudioCallback&>(processor));
	}

	void AttachAudioMixedProcessor(AudioCallback processor)
	{
		::AttachAudioMixedProcessor(reinterpret_cast<::AudioCallback&>(processor));
	}

	void DetachAudioMixedProcessor(AudioCallback processor)
	{
		::DetachAudioMixedProcessor(reinterpret_cast<::AudioCallback&>(processor));
	}
} // namespace ray
