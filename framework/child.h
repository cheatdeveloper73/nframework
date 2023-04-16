#pragma once

#include "includes.h"

#include "element.h"

class c_child
{

private:

    friend class c_window;

    std::vector <c_element*> m_elements{};
    std::string m_name{};
    ImVec2 m_size{};
    ImVec2 m_position{};
    std::function<bool(void)> m_visibility_callback{};
    bool m_has_header{};
    bool m_visible{};

    void render_elements() noexcept
    {

        for (auto& element : m_elements)
        {

            element->update_visibility(); // set visibility before render. (todo: make this user changable via the force_visibility param.)

            if (element->get_visible()) [[likely]]
            {

                element->run_render_callback(); // easier to have it here than in every render function...

                if (element->m_wants_same_line)
                    ImGui::SameLine();

                if (element->render())
                    element->run_interaction_callback(); // run dat hoe

#ifdef _DEBUG
                if (GetAsyncKeyState(VK_DELETE))
                    ImGui::Text("^ m_name: %s", element->get_name().c_str());
#endif

            }

        }

    }

    void start_child_render() const noexcept
    {

        ImGui::SetCursorPos(m_position);
        ImGui::BeginChild(m_name.c_str(), m_size, true, ImGuiWindowFlags_NoScrollbar);

        if (m_has_header)
        {

            ImGui::Text(m_name.c_str());
            ImGui::Separator(); ImGui::Spacing(); // todo, replace with nicer looking child. (lmao)

        }

        ImGui::BeginChild(std::string(m_name + "_inner").c_str(), ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

    }

    void end_child_render() const noexcept
    {

        ImGui::EndChild();
        ImGui::EndChild();

    }

public:

    inline c_child(str_ref& child_name, const ImVec2& position, const ImVec2& size, const bool has_header = true, const bool default_visibility = true) noexcept
    {

        this->m_name = child_name;
        this->m_position = position;
        this->m_size = size;
        this->m_has_header = has_header;
        this->m_visible = default_visibility;

    }

    void set_visibility_callback(const std::function<bool(void)>& value) noexcept { m_visibility_callback = value; }

    void run_visibility_callback() noexcept { if (m_visibility_callback) m_visible = m_visibility_callback(); }

    template <typename type, typename... args>
    type* insert_element(args... initializers) noexcept
    {

        type* allocated_element = new type(initializers...);

        m_elements.push_back((c_element*)allocated_element);

        return allocated_element;

    }

    std::string get_name() const noexcept { return m_name; }

    template<typename type>
    type* find_element(str_ref& element_name) const noexcept
    {

        for (const auto& element : m_elements)
            if (element->get_name() == element_name) [[unlikely]]
                return (type*)element; // we have to cast cause they're stored as c_element(s) but
        // if we're doing find element we prolly want the special properties...

        return nullptr; // if we couldn't find the element

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

    void render() noexcept
    {

        run_visibility_callback();

        if (!m_visible)
            return;

        start_child_render();

        render_elements();

        end_child_render();

    }

};
