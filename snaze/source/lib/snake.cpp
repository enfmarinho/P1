#include "snake.h"
#include "level.h"

Snake::~Snake() { delete m_level; }

void Snake::initialize(short lives, short foods) {
  m_lives_remaining = lives;
  m_foods_per_level = foods;
  m_foods_remaining = foods;
}

void Snake::move(Snake::direction_e direction) {
  m_direction = direction;
  auto head = m_body.front();
  Level::position_e next_pos;
  if (direction == direction_e::DOWN) {
    next_pos = m_level->consult(++head.y, head.x);
  } else if (direction == direction_e::UP) {
    next_pos = m_level->consult(--head.y, head.x);
  } else if (direction == direction_e::RIGHT) {
    next_pos = m_level->consult(head.y, ++head.x);
  } else {
    next_pos = m_level->consult(head.y, --head.x);
  }

  if (next_pos == Level::position_e::FOOD) {
    append(direction);
  } else if (next_pos == Level::position_e::WALL or
             next_pos == Level::position_e::BODY or
             next_pos == Level::position_e::HEAD) {
    m_alive = false;
    --m_lives_remaining;
  } else if (m_body.size() == 1) {
    m_level->assign(m_body.front().y, m_body.front().x,
                    Level::position_e::FREE);
    m_body.pop_front();
    m_body.push_front(head);
    m_level->assign(head.y, head.x, Level::position_e::HEAD);
  } else {
    m_level->assign(m_body.back().y, m_body.back().x, Level::position_e::FREE);
    m_body.pop_back();
    m_level->assign(m_body.front().y, m_body.front().x,
                    Level::position_e::BODY);
    m_body.push_front(head);
    m_level->assign(head.y, head.x, Level::position_e::HEAD);
  }
}

void Snake::append(direction_e direction) {
  Level::coordinate head = m_body.front();
  m_level->assign(head.y, head.x, Level::position_e::BODY);
  if (direction == direction_e::UP) {
    --head.y;
  } else if (direction == direction_e::DOWN) {
    ++head.y;
  } else if (direction == direction_e::RIGHT) {
    ++head.x;
  } else {
    --head.x;
  }
  m_body.push_front(head);
  m_level->assign(head.y, head.x, Level::position_e::HEAD);
  m_level->generates_food();
  m_score += 50 * (m_foods_per_level - get_foods_remaining() + 1);
  --m_foods_remaining;
}

void Snake::kill() {
  if (m_alive) {
    --m_lives_remaining;
    m_alive = false;
  }
}

void Snake::assign_level(Level &level) {
  m_foods_remaining = m_foods_per_level;
  restart_level(level);
}

void Snake::restart_level(Level &level) {
  m_alive = true;
  m_level = new Level(level);
  m_body.clear();
  m_body.push_front(m_level->get_spawn());
  m_level->generates_food();
}
