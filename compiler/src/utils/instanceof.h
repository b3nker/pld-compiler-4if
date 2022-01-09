/**
 * Checks if ptr is of type Base
 * @tparam Base
 * @tparam T
 * @param ptr
 * @return
 */
template <typename Base, typename T> inline bool instanceof (const T *ptr) {
    return dynamic_cast<const Base *>(ptr) != nullptr;
}
