#ifndef POSTMAN_HPP
#define POSTMAN_HPP

#include <nano/event.hpp>

#include <functional>
#include <unordered_map>

namespace nano
{

class postman
{
public:
    struct key_t
    {
        nano::event ev;
        int scene_id;
    };

    struct equal_key_t
    {
        bool operator()(const postman::key_t& fst,
                        const postman::key_t& snd) const;
    }; // here equality checks only for needed event types

    struct hash_key_t
    {
        int operator()(const postman::key_t& key) const;
    };

    using eventHandler = std::function<void()>;
    int subscribe(key_t key, eventHandler handler);
    int unsubscribe(key_t key);
    int deliver(nano::event ev);

private:
    std::unordered_map<key_t, eventHandler, hash_key_t, equal_key_t> recipients;
};

} // namespace nano

#endif // POSTMAN_HPP
