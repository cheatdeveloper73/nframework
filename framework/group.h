#pragma once

#include "element.h"

// easy way to group da elemuntaz
class c_group : public c_element
{

private:

    std::vector <c_element*> m_elements{};

public:

    c_group() = default;

    inline c_group(str_ref& name, str_ref& label, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::GROUP;

    }

    template <typename type, typename... args>
    type* insert_element(args... initializers) noexcept
    {

        type* allocated_element = new type(initializers...);

        m_elements.push_back((c_element*)allocated_element);

        return allocated_element;

    }

    bool pop_element(str_ref& element_name) noexcept
    {

        for (const auto& element : m_elements)
            if (element->get_name() == element_name) [[likely]]
            {

                const auto position = std::find(m_elements.begin(), m_elements.end(), element);

                if (position != m_elements.end()) [[unlikely]]
                    m_elements.erase(position);

                return true;

            }

        return false;

    }

    virtual bool render()
    {

        // todo; this system doesn't really work, I feel like in theory it should but it doesn't. someone, please. fix it for me....
        int multiplier = 1;

        for (const auto& element : m_elements)
            if (element->m_type != element_type::KEYBIND)
                ++multiplier;

        // someone fix dis 4 me...
        if (const bool ret = ImGui::BeginChild(m_label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, (30 * multiplier)), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize))
        {

            ImGui::Text(m_label.c_str()); ImGui::Spacing();

            ImGui::BeginGroup();

            for (const auto& element : m_elements)
            {

                // properly handle rendering...

                element->run_render_callback();

                if (element->m_wants_same_line)
                    ImGui::SameLine();

                if (element->render())
                    element->run_interaction_callback();

#ifdef _DEBUG
                if (GetAsyncKeyState(VK_DELETE))
                    ImGui::Text("^ m_name: %s", element->get_name().c_str());
#endif

            }

            ImGui::EndGroup();

            ImGui::EndChild();

            return ret;

        }

        return false;

    }

};
