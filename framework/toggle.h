#pragma once

#include "element.h"

// dis one goes out to my homie t0ggle

class c_toggle : public c_element
{

private:

    bool* m_value{};

public:

    inline c_toggle(str_ref& name, str_ref& label, bool* value, const bool default_visibility = true) noexcept
    {

        this->m_value = value;
        this->m_name = name;
        this->m_label = label;
        this->m_visible = default_visibility;
        this->m_type = element_type::SWITCH;

    }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str()); ImGui::SameLine(ImGui::GetContentRegionAvail().x - (12.f * 2.f));

        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        const float height = 15.f;
        const float width = height * 1.5f;
        const float radius = height * 0.5f;

        const bool value_modified = ImGui::InvisibleButton(m_name.c_str(), ImVec2(width, height), ImGuiButtonFlags_AlignTextBaseLine);

        const float end_x = p.x + width - radius;
        const float base_x = p.x + radius;

        if (value_modified)
            *m_value = !*m_value;

        ImU32 col_bg;

        const bool hovered = ImGui::IsItemHovered();

        if (hovered)
            col_bg = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
        else
            col_bg = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, ImGui::GetStyle().FrameRounding); // frame
        draw_list->AddRect(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)), ImGui::GetStyle().FrameRounding); // border
        draw_list->AddCircleFilled(ImVec2(*m_value ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255)); // slider

        return value_modified;

    }

};
