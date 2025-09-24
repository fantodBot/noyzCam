# noyzCam
A high specificity noisy vehicle tracking camera using RPi 5 Python cv2 for video and ESP32S2 Arduino for stereo and 2D noise direction estimation
# Hardware:
[Raspberry Pi 5](https://www.digikey.com/en/products/detail/raspberry-pi/SC1431/21658261)  
[RPi Active Cooler](https://www.digikey.com/en/products/detail/raspberry-pi/SC1148/21658255)  
[RPi 27W USB Power Supply](https://www.digikey.com/en/products/detail/raspberry-pi/SC1153/21658276)  
[RPi Camera Module 3 Wide](https://www.digikey.com/en/products/detail/raspberry-pi/SC1224/17278644)  
[RPi Global Shutter Camera](https://www.digikey.com/en/products/detail/raspberry-pi/SC1222/17877577)  
[RPi 500mm Camera Ribbon Cable](https://www.digikey.com/en/products/detail/raspberry-pi/SC1130/21658263)  
[microSD card](https://www.digikey.com/en/products/detail/raspberry-pi/SC1629/24627140)  
[Wensn WS1361 SPL Meter](https://www.google.com/aclk?sa=L&ai=DChsSEwiW0eWTu9uPAxV8R_8BHf8-F08YACICCAEQBxoCbWQ&co=1&ase=2&gclid=Cj0KCQjw8p7GBhCjARIsAEhghZ3OvpxGtcm2b47yXEvnsvshKW3kaDa1tB6qAs6d7fi4Hx0BJEoC0X8aAtVnEALw_wcB&cid=CAASZuRoe5tyFzmPZUO3uZQsLEV2wiqCWcEYx2KxfCWjoDLwatLPTdEF0vLGeo6WzykZ517eqXecVaV6CfLVxpNhbVbAeLPwKsRi2BeMAurKB1GCysTp0oGPNbcHDOGDbHE64G4GjZ8-pg&cce=2&category=acrcp_v1_32&sig=AOD64_25wu4pPWu0hyUzTM_lJ2f4gZb6Aw&ctype=5&q=&nis=4&ved=2ahUKEwjJ5uCTu9uPAxVQ1fACHbWJIJ0Q9aACKAB6BAgKECk&adurl=)  
[Mini Digital Servo](https://www.amazon.com/dp/B0C94DFN6X?ref=ppx_yo2ov_dt_b_fed_asin_title)  
[5-100mm CS Mount Camera Lens](https://www.aliexpress.us/item/3256804302386573.html?spm=a2g0o.productlist.main.1.47f4Tu2fTu2fbc&algo_pvid=1db0b795-a8cd-4eeb-b4aa-66bb932829da&algo_exp_id=1db0b795-a8cd-4eeb-b4aa-66bb932829da-0&pdp_ext_f=%7B%22order%22%3A%2263%22%2C%22eval%22%3A%221%22%7D&pdp_npi=6%40dis%21USD%2141.02%2124.52%21%21%21290.49%21173.63%21%402101e7f617579652163102183ed36f%2112000029334059965%21sea%21US%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A90b9bb98%3Bm03_new_user%3A-29895%3BpisId%3A5000000174474666&curPageLogUid=QgbfS4rYbVD1&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005004488701325%7C_p_origin_prod%3A)  
[Wemos S2 Mini](https://www.aliexpress.us/item/3256806798651168.html?spm=a2g0o.productlist.main.1.538f3a948Se23P&algo_pvid=fb54dd2b-e78e-41b0-8e57-23417bc2c186&algo_exp_id=fb54dd2b-e78e-41b0-8e57-23417bc2c186-0&pdp_ext_f=%7B%22order%22%3A%22379%22%2C%22eval%22%3A%221%22%7D&pdp_npi=6%40dis%21USD%215.45%210.99%21%21%2138.63%217.07%21%402101ea8c17579654840091352e9307%2112000038946834380%21sea%21US%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A90b9bb98%3Bm03_new_user%3A-29895%3BpisId%3A5000000174474738&curPageLogUid=OMwsA8ifIU0J&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006984965920%7C_p_origin_prod%3A)  
[MCP6022 Op Amp](https://www.digikey.com/en/products/detail/microchip-technology/MCP6022-E-P/683219)  
[AMM-2738-B-EB-R MEMS Mic Breakout](https://www.digikey.com/en/products/detail/pui-audio-inc/AMM-2738-B-EB-R/12315318) x 2  
