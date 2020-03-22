#include "Camera.hpp"

#include "../Arth/Math.hpp"

Camera::Camera(Graphics &gfx, Input &input, sf::Vector2f *toFollow)
    : m_gfx(gfx),
      m_input(input),
      m_pos(0.0f, 0.0f),
      m_toFollow(toFollow),
      m_zoom(1.0f, 1.0f),
      m_angle(0.0f),
      m_rotationSpeed(5.0f)
{
}

void Camera::Update(sf::Time dt)
{
    if (m_input.Get(KeyQ))
        m_angle += m_rotationSpeed * dt.asSeconds();
    if (m_input.Get(KeyE))
        m_angle -= m_rotationSpeed * dt.asSeconds();

    const sf::Vector2i cur_pos = sf::Mouse::getPosition(m_gfx.GetRenderWindow());
    if (m_input.Get(Code::MouseL) && m_input.Get(Code::MouseR))
    {
        m_engaged = true;
        sf::Vector2i delta = cur_pos - m_lastPos;
        if (vf::LengthSq((sf::Vector2f)delta) > 0.0f)
        {
            m_lastPos = cur_pos;
            delta = (sf::Vector2i)m_gfx.AddDefaultRotation((sf::Vector2f)delta);
            delta = (sf::Vector2i)m_gfx.AddDefaultScale((sf::Vector2f)delta);
            delta *= -1;
            m_pos += (sf::Vector2f)delta;
        }
    }
    else
    {
        m_lastPos = cur_pos;
        m_engaged = false;
    }
    if (m_input.Get(KeyR))
        ResetTransformation();

    CapZoomLevel();
}

void Camera::PushChain(const sf::Drawable &drawable)
{
    const sf::Vector2f offset(float(Graphics::ScreenWidth / 2), float(Graphics::ScreenHeight / 2));
    sf::Transform t = sf::Transform::Identity;
    t.translate(offset);
    t.rotate(m_angle);
    t.scale(m_zoom);
    t.translate(-m_pos);
    m_gfx.ApplyTransformation(t);
    m_gfx.Render(drawable);
}

sf::Rect<float> Camera::GetViewportRect() const
{
    const sf::Vector2f zoom = sf::Vector2f(1.0f / m_zoom.x, 1.0f / m_zoom.y);
    const float diagonal = sqrt(
        pow(float(Graphics::ScreenWidth / 2) * zoom.x, 2) +
        pow(float(Graphics::ScreenHeight / 2) * zoom.y, 2));
    return sfmlext::RectFromCenter(m_pos, diagonal, diagonal);
}

void Camera::CapZoomLevel()
{
    gf::Constrain(m_zoom.x, 0.9f, 3.0f);
    gf::Constrain(m_zoom.y, 0.9f, 3.0f);
}

void Camera::ResetTransformation()
{
    m_pos = sf::Vector2f(0.0f, 0.0f);
    m_angle = 0.0f;
    m_zoom = sf::Vector2f(1.0f, 1.0f);
}