# NFC Business Card (Day 12)

A business card that opens my whole portfolio when you tap it on a phone. No app, no typing, just hold the card to the back of the phone and my Mi Robo page opens.

## What it does

- Tap the card on an NFC phone and `https://mi-robo.github.io` opens in the browser.
- That page has my Instagram, YouTube, TikTok, GitHub and my latest build.
- The link can be changed any time, because the tag is rewritable.

## Parts

| Part | Notes |
| --- | --- |
| NTAG213 NFC sticker | About 144 bytes, enough for one link. I bought mine from Robotech Shop for around ৳24 to ৳30 each. |
| A blank PVC card | Only used as a printable body. Its own chip is ignored. |
| An NFC phone | Android works instantly. iPhone XS and newer also works. |
| NFC Tools app (by wakdev) | Free, used to read and write the tag. |

No soldering and no microcontroller needed for this one.

## How it works

1. NFC runs at 13.56 MHz. The tag has no battery. The phone's field powers it for a moment.
2. The tag stores a small NDEF record that holds a URL.
3. The phone reads the record and opens the link.

A tag holds one link, so I point it to a portfolio page that holds all my links.

## Steps

1. Build a portfolio page and put it online (I used GitHub Pages, repo `mi-robo.github.io`).
2. Open NFC Tools, go to **Read**, and tap the sticker to confirm it says **NTAG213**.
3. Go to **Write**, then **Add a record**, then **URL**. Enter the link and choose `https://`.
4. Tap **Write** and hold the sticker to the back of the phone.
5. Close the app and tap the sticker again. The page should open by itself.
6. Test the sticker on the card before pressing it down firmly.
7. Print the design on the front of the card, stick the tag on the back, and mark the spot with a small tap icon.
8. Optional: lock the tag as read-only once everything works. Locking is permanent.

## What did not work (so you can skip it)

- **MIFARE Classic 1K cards.** These are the common cheap "RFID" cards. Phones cannot use them as link cards, and iPhones cannot read them at all. Buy NTAG213 or NTAG215.
- **125 kHz ID cards.** I had a pair of these. The phone does nothing when you tap them. They need a separate 125 kHz reader like the RDM6300.
- **Sticker on top of a card with its own coil.** My sticker read instantly alone but felt laggy on the card. If that happens, add a few layers of thick paper between the sticker and the card, move the sticker toward a corner, or use a plain card with no chip.
- **Metal.** Metallic foil or ink over the tag blocks the signal.

## Tips

- Test the card on both Android and, if you can, an iPhone.
- Keep the tag centered and flat. Bubbles and bends weaken the read.
- Buy 3 to 5 stickers so you have spares for experiments.
- Check that the listing says NTAG213 or NTAG215 before buying. Skip anything that says "S50", "MIFARE 1K" or just "RFID card".

## Links

- Live page: https://mi-robo.github.io
- Instagram: [@mi_robo_](https://www.instagram.com/mi_robo_)
- YouTube: [@mi-robo](https://www.youtube.com/@mi-robo)
- TikTok: [@mi_robo_](https://www.tiktok.com/@mi_robo_)

Part of the Mi Robo Projects series. If you build your own, tell me how it went.
