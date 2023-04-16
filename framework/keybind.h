#pragma once

#ifdef ENABLE_BINDS

// this is going to be a mess

#include "includes.h"
#include "element.h"
#include "../imgui_internal.h"

#include <Windows.h>

enum keybind_display_type : uint16_t
{

    WITH_LABEL,
    WITHOUT_LABEL

};

enum keybind_interaction_type : uint16_t
{

    TOGGLE,
    HELD,
    ALWAYS,
    NEVER

};

const char* const ___key_names[] = {
    "Unknown",
    "VK_LBUTTON",
    "RBUTTON",
    "CANCEL",
    "MBUTTON",
    "XBUTTON1",
    "XBUTTON2",
    "Unknown",
    "BACK",
    "TAB",
    "Unknown",
    "Unknown",
    "CLEAR",
    "RETURN",
    "Unknown",
    "Unknown",
    "SHIFT",
    "CONTROL",
    "MENU",
    "PAUSE",
    "CAPITAL",
    "KANA",
    "Unknown",
    "JUNJA",
    "FINAL",
    "KANJI",
    "Unknown",
    "ESCAPE",
    "CONVERT",
    "NONCONVERT",
    "ACCEPT",
    "MODECHANGE",
    "SPACE",
    "PRIOR",
    "NEXT",
    "END",
    "HOME",
    "LEFT",
    "UP",
    "RIGHT",
    "DOWN",
    "SELECT",
    "PRINT",
    "EXECUTE",
    "SNAPSHOT",
    "INSERT",
    "DELETE",
    "HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "Unknown",
    "SLEEP",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "MULTIPLY",
    "ADD",
    "SEPARATOR",
    "SUBTRACT",
    "DECIMAL",
    "DIVIDE",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "NUMLOCK",
    "SCROLL",
    "OEM_NEC_EQUAL",
    "OEM_FJ_MASSHOU",
    "OEM_FJ_TOUROKU",
    "OEM_FJ_LOYA",
    "OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "LSHIFT",
    "RSHIFT",
    "LCONTROL",
    "RCONTROL",
    "LMENU",
    "RMENU"
};


// class designed to represent a bindable key on the keyboard (THIS SUCKS!!!!, works though.)
class c_binding
{

private:

    friend class c_keybind; // I am so sorry.

    std::string m_name{};
    keybind_interaction_type m_type{ keybind_interaction_type::NEVER };
    int m_current_key{ NULL };
    bool m_enabled{ false };
    bool m_waiting_for_input{ false };

    bool bind_new_key() noexcept
    {

        // if this errors replace it with "IsKeyPressedMap"
        if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
        {

            m_current_key = NULL;
            ImGui::ClearActiveID(); // if this errors, replace with "ClearActiveID"
            return true;

        }

        for (auto i = 1; i < 5; i++)
        {

            if (ImGui::GetIO().MouseDown[i])
            {

                switch (i)
                {
                case 1:
                    m_current_key = VK_RBUTTON;
                    break;
                case 2:
                    m_current_key = VK_MBUTTON;
                    break;
                case 3:
                    m_current_key = VK_XBUTTON1;
                    break;
                case 4:
                    m_current_key = VK_XBUTTON2;
                    break;
                }
                return true;

            }

        }

        for (auto i = VK_BACK; i <= VK_RMENU; i++)
        {

            if (ImGui::GetIO().KeysDown[i])
            {

                m_current_key = i;
                return true;

            }

        }

        return false;

    }

public:

    inline c_binding(str_ref& name, const int default_key = NULL) noexcept
    {

        this->m_name = name;
        this->m_current_key = default_key;
        this->m_type = keybind_interaction_type::HELD;

    }

    std::string get_key_name() const noexcept
    {

        if (!m_current_key)
            return "None";

        std::string tmp = ___key_names[m_current_key]; // there's no issues here... right?

        std::transform(tmp.begin(), tmp.end(), tmp.begin(),
            [](unsigned char c) { return std::tolower(c); });

        tmp[0] = std::toupper(tmp[0]);

        return tmp;

    }

    std::string get_bind_name() const noexcept { return m_name; }

    keybind_interaction_type get_bind_type() const noexcept { return m_type; }

    int get_bound_key() const noexcept { return m_current_key; }

    bool get_enabled() const noexcept { return m_enabled; }

    void update_state() noexcept
    {

        switch (m_type)
        {

        case keybind_interaction_type::ALWAYS:
            m_enabled = true;
            break;
        case keybind_interaction_type::HELD:
            m_enabled = GetAsyncKeyState(m_current_key);
            break;
        case keybind_interaction_type::TOGGLE:
            if (GetAsyncKeyState(m_current_key) & 1) // todo: replace
                m_enabled = !m_enabled;
            break;
        case keybind_interaction_type::NEVER:
            m_enabled = false;
            break;

        }

    }

};

// the element which displays the interface for bindings
class c_keybind : public c_element
{

private:

    c_binding* m_bind{};
    keybind_display_type m_display_type{};

public:

    c_keybind(str_ref& name, str_ref& label, c_binding* bind, keybind_display_type display_type = keybind_display_type::WITH_LABEL, const bool default_visibility = true)
    {

        this->m_name = name;
        this->m_label = label;
        this->m_bind = bind;
        this->m_display_type = display_type;
        this->m_visible = default_visibility;
        this->m_type = element_type::KEYBIND;

    }

    // unholy function
    virtual bool render()
    {

        static const auto a_sz = ImGui::CalcTextSize("A").y;
        static const auto h_sz = ImGui::CalcTextSize("H").y;
        static const auto t_sz = ImGui::CalcTextSize("T").y;

        static const auto window_size = ImVec2{71, (a_sz + h_sz + t_sz + a_sz + a_sz + 8)}; // change this if you wanna change the window size.
        // ^ THIS IS SO CANCER SOMEONE PLEASE FIX

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        if (m_display_type == keybind_display_type::WITH_LABEL)
        {

            ImGui::Text(m_label.c_str()); // cheapest shit ever.. (funny enough it properly works too)
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 66.f);

        }
        else
        {

            const ImVec2 last_rect_max = ImGui::GetItemRectMax();
            const ImVec2 last_rect_min = ImGui::GetItemRectMin();

            ImGui::SetCursorPosX((last_rect_min.x - ImGui::GetWindowPos().x) - ((180.f / 3.f) + 6.f));
            ImGui::SetCursorPosY((last_rect_min.y - ImGui::GetWindowPos().y) - 2.f);

        }

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(m_bind->get_bind_name().c_str());
        const ImVec2 label_size = ImGui::CalcTextSize(m_bind->get_bind_name().c_str());

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(ImVec2(0, 0), 60.f, label_size.y + style.FramePadding.y * 1.5f);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

        bool value_changed = false;
        int key = m_bind->get_bound_key();

        auto io = ImGui::GetIO();

        std::string name = m_bind->get_key_name();

        // the thing that shows when you're waiting for a key to be pressed
        if (m_bind->m_waiting_for_input)
            name = "...";

        if (ImGui::GetIO().MouseClicked[0] && hovered)
        {

            if (g.ActiveId == id)
            {

                m_bind->m_waiting_for_input = true;

            }

        }
        else if (ImGui::GetIO().MouseClicked[1] && hovered) {
            ImGui::OpenPopup(m_bind->get_bind_name().c_str());
        }
        else if (ImGui::GetIO().MouseClicked[0] && !hovered) {
            if (g.ActiveId == id)
                ImGui::ClearActiveID();
        }

        if (m_bind->m_waiting_for_input)
            if (m_bind->bind_new_key())
            {

                ImGui::ClearActiveID();
                m_bind->m_waiting_for_input = false;

            }

        // render dat hoe

        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg)), ImGui::GetStyle().FrameRounding);

        if (ImGui::GetStyle().FrameBorderSize)
            window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)), ImGui::GetStyle().FrameRounding);

        const auto text_sz = ImGui::CalcTextSize(name.c_str());
        const auto centered_x = (size.x - text_sz.x) * 0.5f; // divide by 2
        const auto centered_y = (size.y - text_sz.y) * 0.5f; // divide by 2
        // ^ cancer above bcs i'm incapable of using imgui math ops

        // when waiting for input
        if (m_bind->m_waiting_for_input)
            window->DrawList->AddText(ImVec2(bb.Min.x + centered_x, bb.Min.y + centered_y), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)), name.c_str());
        else
            window->DrawList->AddText(ImVec2(bb.Min.x + centered_x, bb.Min.y + centered_y), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), name.c_str());

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + 21));
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));

        {

            if (ImGui::BeginPopup(m_bind->get_bind_name().c_str(), window_flags)) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f); {
                    ImGui::SetCursorPos(ImVec2(6, 7)); {
                        ImGui::BeginGroup(); {
                            if (ImGui::Selectable("On", m_bind->m_type == keybind_interaction_type::ALWAYS)) m_bind->m_type = keybind_interaction_type::ALWAYS;
                            if (ImGui::Selectable("Held", m_bind->m_type == keybind_interaction_type::HELD)) m_bind->m_type = keybind_interaction_type::HELD;
                            if (ImGui::Selectable("Toggle", m_bind->m_type == keybind_interaction_type::TOGGLE)) m_bind->m_type = keybind_interaction_type::TOGGLE;
                            if (ImGui::Selectable("Off", m_bind->m_type == keybind_interaction_type::NEVER)) m_bind->m_type = keybind_interaction_type::NEVER;
                        } ImGui::EndGroup();
                    }
                } ImGui::PopStyleVar();
                ImGui::EndPopup();
            }
        }

        return pressed;

    }

    c_binding* get_bind() const { return m_bind; }

    keybind_display_type get_display_type() const { return m_display_type; }

};

#endif
