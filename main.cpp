// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Mã chính
int main(int, char**)
{
    // Tạo cửa sổ ứng dụng
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Ví dụ Dear ImGui", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Ví dụ Dear ImGui DirectX9", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Khởi tạo Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Hiển thị cửa sổ
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Thiết lập ngữ cảnh Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Bật điều khiển bằng bàn phím
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Bật điều khiển bằng gamepad

    // Thiết lập kiểu Dear ImGui
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Thiết lập giao diện và máy chủ trình kết xuất
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Tải các phông chữ
    io.Fonts->AddFontDefault();
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 30.0f, nullptr, io.Fonts->GetGlyphRangesVietnamese());
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 25.0f, nullptr, io.Fonts->GetGlyphRangesVietnamese());
    ImFont* font3 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesVietnamese());
    IM_ASSERT(font1 != nullptr);
    IM_ASSERT(font2 != nullptr);
    IM_ASSERT(font3 != nullptr);
    // Trạng thái của chúng ta
    //bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Vòng lặp chính
    bool done = false;
    while (!done)
    {
        // Kiểm tra và xử lý các thông điệp (đầu vào, thay đổi kích thước cửa sổ, v.v.)
        // Xem hàm WndProc() phía dưới để biết cách gửi sự kiện đến giao diện Win32.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Xử lý thay đổi kích thước cửa sổ (chúng ta không thay đổi trực tiếp trong xử lý WM_SIZE)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Bắt đầu khung của Dear ImGui
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Tạo một cửa sổ có tên "Xin chào, thế giới!" và thêm nội dung vào đó.

            ImGui::Text("This is some useful text.");               // Hiển thị một số văn bản (bạn có thể sử dụng chuỗi định dạng).

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Chỉnh sửa số thực bằng cách sử dụng thanh trượt từ 0.0f đến 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Chỉnh sửa 3 số thực biểu thị một màu

            if (ImGui::Button("Button"))                           // Nút trả về true khi được nhấp (hầu hết các tiện ích trả về true khi chỉnh sửa/kích hoạt)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

        // Hiển thị dòng chữ "Hello, world!" trong cửa sổ
        ImGui::Begin("User Interface");
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(193.0f / 255.0f, 228.0f / 255.0f, 205.0f / 255.0f, 1.0f);
        ImGui::PushFont(font1);
        ImGui::TextColored(ImVec4(38.0f / 255.0f, 95.0f / 255.0f, 42.0f / 255.0f, 1.0f), "Appeal Application Form");
        ImGui::PopFont();
        ImGui::PushFont(font2);
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Personal Information");
        ImGui::PopFont();
        float cornerRounding = 5.0f; // Độ bo góc mong muốn

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, cornerRounding);
        ImGui::PushFont(font3);
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Name");
        
        char inputBufferfirt[256] = "";
        char inputBufferlast[256] = "";// Để lưu trữ giá trị nhập vào
        ImVec4 bgColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
        ImVec4 borderColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
        ImVec4 textColor = ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        ImGui::Columns(2, "InputColumns", true);

        // Đặt chiều rộng của cột 1 thành 50% (0.5f) chiều rộng hiện tại
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.5f);

        // Ô nhập dữ liệu - Cột 1
        char inputBuffer1[256] = ""; // Để lưu trữ giá trị nhập vào
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputText("##InputText1", inputBuffer1, sizeof(inputBuffer1));
        // Ô nhập dữ liệu - Cột 2
        char inputBuffer2[256] = ""; // Để lưu trữ giá trị nhập vào
        
        ImGui::NextColumn(); // Di chuyển đến cột tiếp theo
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputText("##InputText2", inputBuffer2, sizeof(inputBuffer2));

        // Kết thúc layout cột
        ImGui::Columns(1);

        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::PushFont(font3);
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Student Number");
        char inputBuffernumber[256] = ""; // Để lưu trữ giá trị nhập vào
        ImGui::SetNextItemWidth(-1.0f);
        ImVec4 bgColor1 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor1);
        ImVec4 borderColor1 = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor1);
        ImVec4 textColor1 = ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor1);
        ImGui::InputText("##InputText3", inputBuffernumber, sizeof(inputBuffernumber));
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::PushFont(font2);
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Appeal Information");
        ImGui::PopFont();

        ImGui::PushFont(font3);
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Request Summary");
        char inputBufferSummary[256] = "";
        char inputBufferUpload[256] = "";// Để lưu trữ giá trị nhập vào
        ImGui::SetNextItemWidth(-1.0f);
        ImVec4 bgColor2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor2);
        ImVec4 borderColor2 = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor2);
        ImVec4 textColor2 = ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor2);
        ImGui::InputTextMultiline("##InputText4", inputBufferSummary, sizeof(inputBufferSummary), ImVec2(-1.0f, 90));
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Letter of Appeal Upload");
        ImGui::InputTextMultiline("##InputText5", inputBufferUpload, sizeof(inputBufferUpload), ImVec2(-1.0f, 60));
        ImGui::TextColored(ImVec4(100.0f / 255.0f, 160.0f / 255.0f, 112.0f / 255.0f, 1.0f), "Do you have supporting documents to upload in support of your appeal?");
        ImGui::SetNextItemWidth(-1.0f);
        const char* items[] = { "Yes", "No" };
        static int selectedItem = -1; // Chưa có mục nào được chọn mặc định
        static const char* displayValue = ""; // Giá trị hiển thị mặc định

        if (selectedItem == -1)
            displayValue = "Yes";
        else
            displayValue = items[selectedItem];

        if (ImGui::BeginCombo("##Combo", displayValue))
        {
            for (int i = 0; i < IM_ARRAYSIZE(items); i++)
            {
                const bool isSelected = (selectedItem == i);
                if (ImGui::Selectable(items[i], isSelected))
                {
                    selectedItem = i;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopStyleColor(3);
        
        ImGui::PopStyleVar();

        ImGui::PopFont();
        


        ImGui::End();
        // Kết xuất
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Xử lý mất thiết bị D3D9
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Tạo đối tượng Direct3D
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Cần sử dụng định dạng cụ thể có chứa alpha nếu cần vẽ alpha per-pixel.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Hiển thị với vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Hiển thị mà không vsync, tốc độ khung hình tối đa
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Tiền khai báo trình xử lý thông điệp từ imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Trình xử lý thông điệp cho Win32
// Bạn có thể đọc các cờ io.WantCaptureMouse, io.WantCaptureKeyboard để xem liệu dear imgui muốn sử dụng đầu vào của bạn hay không.
// - Khi io.WantCaptureMouse là true, hãy không gửi dữ liệu đầu vào chuột đến ứng dụng chính của bạn hoặc xóa/ghi đè dữ liệu chuột.
// - Khi io.WantCaptureKeyboard là true, hãy không gửi dữ liệu đầu vào bàn phím đến ứng dụng chính của bạn hoặc xóa/ghi đè dữ liệu bàn phím.
// Thường thì bạn có thể chuyển toàn bộ đầu vào cho dear imgui và ẩn chúng khỏi ứng dụng của bạn dựa trên hai cờ này.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Đặt lại kích thước cửa sổ
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Vô hiệu hóa menu ứng dụng ALT
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
