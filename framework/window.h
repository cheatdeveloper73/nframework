#pragma once

#include "includes.h"

#include "element.h"
#include "child.h"
#include "keybind.h"
#include "attachment.h"

class c_window
{

private:

    std::vector <c_element*> m_elements{};
    std::vector <c_child*> m_children{};
    std::vector <c_attachment*> m_attachments{};
    std::function<bool(void)> m_visibility_callback{};
    std::string m_name{};
    ImVec2 m_size{};
    int m_flags{};
    bool m_visible{};

    void render_children() const noexcept
    {

        for (const auto& child : m_children)
            child->render();

    }

#ifdef ENABLE_BINDS

    std::vector<c_binding*> get_all_binds() const noexcept
    {

        // note; type system for elements was made because of this
        // relying on rtti is sussy...
        // having a type system is pretty helpful though.

        std::vector<c_binding*> tmp_vec{};

        // read all keybind elements inside of our current window
        for (const auto& element : m_elements)
            if (element->m_type == element_type::KEYBIND)
                tmp_vec.push_back(((c_keybind*)element)->get_bind());

        // loop through children to get keybinds (if we have any)
        for (const auto& child : m_children)
            for (const auto& element : child->m_elements)
                if (element->m_type == element_type::KEYBIND)
                    tmp_vec.push_back(((c_keybind*)element)->get_bind());

        return tmp_vec;

    }

#endif

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

    void start_window_render() noexcept { ImGui::SetNextWindowSize(m_size);  ImGui::Begin(m_name.c_str(), NULL, m_flags | ImGuiWindowFlags_NoSavedSettings); }
    void end_window_render() noexcept { ImGui::End(); }

    void update_visibility(const bool force_visibility = false) noexcept
    { // make this functional lol (idk what I was thinking here)

        if (!force_visibility && m_visibility_callback) // automatically set visibility based on the callback, else force visibility via parameter.
            m_visible = m_visibility_callback();
        else if (!force_visibility && m_visibility_callback)
            m_visible = force_visibility;

    }

    void render_attachments() const noexcept
    {

        for (const auto& attachment : m_attachments)
            attachment->render(ImGui::GetWindowPos(), ImGui::GetWindowSize(), ImGui::GetBackgroundDrawList());

    }

public:

    inline c_window(str_ref& window_name, const ImVec2 window_size = ImVec2(0, 0), const int window_flags = NULL, const bool default_visible = true) noexcept
    {

        this->m_name = window_name;
        this->m_flags = window_flags;
        this->m_visible = default_visible;
        this->m_size = window_size;

    }

    bool get_visibility() const noexcept { return m_visible; }
    void set_visibility(const bool value) noexcept { m_visible = value; }

    void set_visibility_callback(const std::function<bool(void)>& value) noexcept { m_visibility_callback = value; }

    void render() noexcept
    {

        // mfw OOP wrapper around immediate mode ui lib...

#ifdef ENABLE_BINDS
        auto binds = get_all_binds();

        if (binds.size() > 0)
            for (auto& bind : binds)
                bind->update_state(); // can't be const because this function modifies values...
#endif

        // update this window's visibility
        update_visibility();

        if (m_visible) [[likely]] // yessir...
        {

            start_window_render();

            render_attachments();

            render_children();

            render_elements();

            end_window_render();

        }

    }

    template <typename type, typename... args>
    type* insert_element(args... initializers)
    {

        type* allocated_element = new type(initializers...);

        m_elements.push_back((c_element*)allocated_element);

        return allocated_element;

    }

    bool insert_attachment(const std::function<void(const ImVec2&, const ImVec2&, ImDrawList*)>& fn)
    {

        c_attachment* allocated_attachment = new c_attachment(fn);

        m_attachments.push_back(allocated_attachment);

        return true;

    }

    c_child* insert_child(str_ref& child_name, const ImVec2& position, const ImVec2& size, bool has_header = true)
    {

        c_child* allocated_child = new c_child(child_name, position, size, has_header);

        m_children.push_back(allocated_child);

        return allocated_child;

    }

    c_child* find_child(str_ref& child_name) const noexcept
    {

        for (const auto& child : m_children)
            if (child->get_name() == child_name) [[unlikely]]
                return child;

        return nullptr; // if we couldn't find the child

    }

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

    size_t get_element_count() const noexcept { return m_elements.size(); }

};
