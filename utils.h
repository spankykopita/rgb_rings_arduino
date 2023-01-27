long mapToByteRange(long value, long fromLow, long fromHigh) {
  return map(value, fromLow, fromHigh, 0, 255);
}

long mapFromByteRange(long value, long toLow, long toHigh) {
  return map(value, 0, 255, toLow, toHigh);
}

CRGBPalette16 getRandomPalette() {
  switch (random8(9))
  {
  case 0:
    return CRGBPalette16(CRGB::Red, CRGB::Orange, CRGB::Violet);
  case 1:
    return CRGBPalette16(CRGB::Cyan, CRGB::LightSeaGreen, CRGB::BlueViolet, CRGB::Red);
  case 2:
    return CRGBPalette16(CRGB::LightSkyBlue, CRGB::LightCoral, CRGB::MediumVioletRed);
  case 4:
    return CRGBPalette16(CRGB::Fuchsia, CRGB::DeepPink, CRGB::HotPink, CRGB::Salmon);
  case 5:
    return RainbowColors_p;
  case 6:
    return PartyColors_p;
  case 7:
    return HeatColors_p;
  }
  return RainbowStripeColors_p;
}