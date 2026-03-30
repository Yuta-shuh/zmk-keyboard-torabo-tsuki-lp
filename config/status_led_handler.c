#include <zephyr/kernel.h>
#include <drivers/led.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>

// 1. LEDデバイスの取得（DeviceTreeで定義した "status_led" を探す）
static const struct device *led_dev = DEVICE_DT_GET(DT_NODELABEL(status_led));

// 2. プロファイルが変わった時に実行される中身
int profile_changed_handler(const zmk_event_t *eh) {
    // 現在のプロファイル番号（0, 1, 2...）を取得
    uint8_t profile_index = zmk_ble_active_profile_index();

    // プロファイル番号 + 1 回だけ点滅させる
    // (BT 0 なら 1回、BT 1 なら 2回)
    for (int i = 0; i <= profile_index; i++) {
        led_on(led_dev, 0);
        k_msleep(150); // 0.15秒点灯
        led_off(led_dev, 0);
        k_msleep(150); // 0.15秒消灯
    }
    return 0;
}

// 3. ZMKに「プロファイル変更イベント」を監視させる登録
ZMK_LISTENER(profile_changed_handler, profile_changed_handler);
ZMK_SUBSCRIPTION(profile_changed_handler, zmk_ble_active_profile_changed);
