#pragma once

#include "Graphics.hpp"
#include "Input.hpp"

class Camera
{
public:
    Camera(Graphics &gfx, Input &input, sf::Vector2f *toFollow = nullptr);

    void Update(sf::Time dt);

    void PushChain(const sf::Drawable &drawable);

    void SetToFollow(sf::Vector2f *toFollow) { m_toFollow = toFollow; }

    sf::Vector2f GetPos() const { return m_pos; }
    sf::Vector2f GetZoom() const { return m_zoom; }
    float GetAngle() const { return m_angle; }
    sf::Rect<float> GetViewportRect() const;

private:
    void CapZoomLevel();
    void ResetTransformation();

private:
    Graphics &m_gfx;
    Input &m_input;

    sf::Vector2f m_pos;
    sf::Vector2f *m_toFollow;

    sf::Vector2f m_zoom;
    float m_angle;
    float m_rotationSpeed;
    bool m_engaged;
    sf::Vector2i m_lastPos;

public:
    static constexpr int OffsetX = Graphics::ScreenWidth / 2;
    static constexpr int OffsetY = Graphics::ScreenHeight / 2;
    static sf::Vector2i GetOffset() { return sf::Vector2i(OffsetX, OffsetY); };
};