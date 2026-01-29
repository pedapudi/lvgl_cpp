#pragma once

static const char* earth_animation_svg = R"svg(
<svg width="150" height="150" viewBox="0 0 150 150" fill="none" xmlns="http://www.w3.org/2000/svg">
                
                <!-- Background Decoration (Stars) - Coordinates scaled by 0.5 -->
                <circle cx="25" cy="25" r="1" fill="#4FD1C5" class="star"></circle>
                <circle cx="125" cy="20" r="1.5" fill="#63B3ED" class="star"></circle>
                <circle cx="135" cy="125" r="1" fill="#F6E05E" class="star"></circle>
                <circle cx="15" cy="110" r="1.25" fill="#F687B3" class="star"></circle>

                <!-- The Globe Group -->
                <g clip-path="url(#globeClip)">
                    <!-- Ocean Background -->
                    <circle cx="75" cy="75" r="50" fill="#3182CE"></circle>
                    
                    <!-- Continents Container -->
                    <g class="earth-spin">
                        
                        <!-- Map Copy 1 (Start at 0) -->
                        <g transform="translate(0, 0)">
                             <!-- Americas -->
                             <!-- Original M90 120 -> M45 60 -->
                            <path d="M45 60 C 40 55, 35 60, 30 65 C 25 70, 27.5 80, 32.5 85 C 35 90, 40 100, 45 105 C 50 100, 52.5 90, 50 80 C 47.5 75, 52.5 70, 55 65 Z" fill="#48BB78"></path>
                            
                            <!-- Africa -->
                            <path d="M80 55 C 75 60, 72.5 70, 75 75 C 77.5 85, 85 95, 90 90 C 95 85, 100 80, 95 70 C 92.5 65, 87.5 55, 80 55 Z" fill="#48BB78"></path> 
                            
                        </g>

                    </g> <!-- End Earth Spin -->
                </g> <!-- End Clip Group -->

                <!-- Defs -->
                <defs>
                    <clipPath id="globeClip">
                        <circle cx="75" cy="75" r="50"></circle>
                    </clipPath>
                    <!-- Gradient Scaled: translate(120->60), scale(187->93) -->
                    <radialGradient id="sphereGradient" cx="0" cy="0" r="1" gradientUnits="userSpaceOnUse" gradientTransform="translate(60 60) rotate(51.3402) scale(93.675)">
                        <stop stop-color="white" stop-opacity="0.4"></stop>
                        <stop offset="1" stop-color="black" stop-opacity="0.1"></stop>
                    </radialGradient>
                </defs>
            </svg>
)svg";
